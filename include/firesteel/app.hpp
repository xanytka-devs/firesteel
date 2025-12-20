#ifndef FS_APP_H
#define FS_APP_H

#include <firesteel/internal/embedded.hpp>
#include <firesteel/utils/delta_time.hpp>
#include <firesteel/common.hpp>
#include <firesteel/window.hpp>
#ifdef FS_RENDERER_OGL
#include <firesteel/rendering/opengl/renderer.hpp>
#endif // FS_RENDERER_OGL
#include <firesteel/internal/devview.hpp>
#ifndef FS_NO_REG_TYPES
#include <firesteel/builtin_types.hpp>
#endif // !FS_NO_REG_TYPES
#ifndef FS_NO_REG_DEFAULT_SHADER
#include <firesteel/rendering/shader.hpp>
#endif // !FS_NO_REG_DEFAULT_SHADER

namespace Firesteel {
	class App {
    private:
        double mLastFrameFPS=0, mLastFrame=0;
        int mFrameCount=0;
	public:
		App() {
			LOG_INFO("Initializing Firesteel App.");
		}
		virtual ~App() { }
		void shutdown() {
			window.close();
		}
        // [!DANGEROUS]
        // Immediately errors out of application no matter the cost.
        // This means no `onShutdown()` will be run.
        void critShutdown(const int& tErrorCode=-13,
            const char* tError="Critical shutdown was called") {
            LOG_CRIT(tError);
            exit(tErrorCode);
        }
		virtual int start(const char* tTitle="Firesteel App",
            const uint tWinWidth=800, const uint tWinHeight=600, const WindowState tWinState=WS_NORMAL) {
            LOG(std::string("Firesteel ") + FS_VERSION);
            LOG_STATE("STARTUP");
            //Do some randomization.
            __time64_t long_time;
	        _time64(&long_time);
	        srand(static_cast<uint>(long_time));
            //Preinitialize.
#if !defined(FS_NO_REG_TYPES) || !defined(FS_NO_COMPONENTS)
            auto* reg=TypeRegistry::sInstance();
            reg->registerType<bool>(std::make_unique<BoolTypeHandler>());
            reg->registerType<uint>(std::make_unique<UIntTypeHandler>());
            reg->registerType<int>(std::make_unique<IntTypeHandler>());
            reg->registerType<float>(std::make_unique<FloatTypeHandler>());
            reg->registerType<double>(std::make_unique<DoubleTypeHandler>());
            reg->registerType<std::string>(std::make_unique<StringTypeHandler>());
            reg->registerType<glm::vec2>(std::make_unique<Vec2TypeHandler>());
            reg->registerType<glm::vec3>(std::make_unique<Vec3TypeHandler>());
            reg->registerType<glm::vec4>(std::make_unique<Vec4TypeHandler>());
#endif // !FS_NO_REG_TYPES
#if defined(FS_PRINT_DEBUG_MSGS) && ( !defined(FS_NO_REG_TYPES) || !defined(FS_NO_COMPONENTS) )
            LOG_DBG("Registered built-in types");
#endif // FS_PRINT_DEBUG_MSGS
			onPreInitialize();
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Run preinitialize");
#endif // FS_PRINT_DEBUG_MSGS
            //Create a window.
            window=Window(tWinWidth, tWinHeight);
            if(!window.initialize(tTitle, tWinState)) return -1;
            //Select renderer.
            if(!onRendererSelect()) return -2;
#ifndef FS_NO_REG_DEFAULT_SHADER
            Shader::setDefaultShader(Embedded::defaultShaderVert,Embedded::defaultShaderFrag);
#endif // !FS_NO_REG_DEFAULT_SHADER
            //Final steps.
            CONFIG::checkGlobalFile();
            onInitialize();
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Run initialize");
#endif // FS_PRINT_DEBUG_MSGS
            //Update loop.
            LOG_STATE("UPDATE LOOP");
            while(window.isOpen()) {
#ifdef FS_INCLUDE_NVTX
                nvtx3::scoped_range u{"update"};
#endif // FS_INCLUDE_NVTX
                window.pollEvents();
                //Per-frame time logic.
                double currentFrame=glfwGetTime();
                DeltaTime::sInstance()->set(static_cast<float>(currentFrame-mLastFrame));
                mLastFrame=currentFrame;
                mFrameCount++;
                if(currentFrame - mLastFrameFPS >= 1.0) {
                    DeltaTime::sInstance()->setFps(mFrameCount);
                    mFrameCount=0;
                    mLastFrameFPS=currentFrame;
                }
                if(window.isMinimized()) continue;
                if(updateViewport) enviroment().renderer->setViewportSize(window.getSize());
                window.clearBuffers();
                //ImGui & DevView window.
                if((Keyboard::getKey(KeyCode::LEFT_CONTROL) || Keyboard::getKey(KeyCode::RIGHT_CONTROL))
                    && Keyboard::keyDown(KeyCode::SLASH)) DEVVIEW::sDrawDevView=true;
#ifdef FS_INCLUDE_NVTX
                nvtx3::scoped_range r{"app update & imgui"};
#endif // FS_INCLUDE_NVTX
                enviroment().renderer->imguiNewFrame();
                onUpdate();
                //Draw DevView and finalize update.
                DEVVIEW::draw(DeltaTime::get(), DeltaTime::fps());
                enviroment().renderer->imguiRender(window.ptr());
                window.swapBuffers();
            }
            //Shutdown.
            LOG_STATE("SHUTDOWN");
            onShutdown();
            enviroment().renderer->imguiShutdown();
            //Quitting.
            window.close();
            glfwTerminate();
            LOG_INFO("Window terminated");
            LOG_STATE("QUIT");
            if(Log::sSaveLogs) Log::destroyFileLogger();
            return 0;
		}
        // [!OVERRIDABLE]
        // Runs before any initialization.
		virtual void onPreInitialize() { }
        // [!OVERRIDABLE]
        // Runs after preinitialize. Used to select renderer.
		virtual bool onRendererSelect() {
            //Check for Vulkan.
            bool isVulkan=(glfwVulkanSupported() == 1);
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG(std::string("Vulkan is") + (isVulkan ? "" : "n't") + " supported on current machine.");
#endif // FS_PRINT_DEBUG_MSGS
            //Renderer init.
            enviroment().renderer=std::make_unique<Renderer>();
#ifdef FS_RENDERER_OGL
            enviroment().renderer=std::make_unique<OGLRenderer>();
#endif // FS_RENDERER_OGL
            if(!enviroment().renderer->initialize()) return false;
            enviroment().renderer->loadExtencions();
            enviroment().renderer->printInfo();
            enviroment().renderer->initializeParams();
            enviroment().renderer->imguiInitialize(window.ptr());
            return true;
        }
        // [!OVERRIDABLE]
        // Runs after window and renderer initialization.
		virtual void onInitialize() { }
        // [!OVERRIDABLE]
        // Runs every frame.
		virtual void onUpdate() { }
        // [!OVERRIDABLE]
        // Runs after the window is closed or on window.close()/app.shutdown().
		virtual void onShutdown() { }

        Enviroment& enviroment() { return *(Enviroment::sInstance()); }
		Window window;
        bool updateViewport=true;
	};
}

#endif // !FS_APP_H
