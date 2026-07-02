#include <firesteel/app.hpp>

#include <firesteel/input/keyboard.hpp>
#include <firesteel/input/mouse.hpp>

#include <firesteel/internal/config.hpp>
#include <firesteel/internal/embedded.hpp>
#include <firesteel/util/delta_time.hpp>
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
    App::App() {
#ifdef FS_PRINT_DEBUG_MSGS
        LOG_DBG("Initializing Firesteel App.");
#endif // FS_PRINT_DEBUG_MSGS
    }
    void App::shutdown() {
        window.close();
    }
    void App::critShutdown(const int& tErrorCode, const char* tError) {
        LOG_CRIT(tError);
        exit(tErrorCode);
    }
    int App::start(const char* tTitle, const uint tWinWidth, const uint tWinHeight, const WindowState tWinState) {
        LOG(std::string("Firesteel ") + FS_VERSION);
        LOG_STATE("STARTUP");
        srand(static_cast<int>(glfwGetTime() / 31));
        //Preinitialize.
#if !defined(FS_NO_REG_TYPES) || !defined(FS_NO_COMPONENTS)
        auto* reg = TypeRegistry::sInstance();
        reg->registerType<bool>(std::make_unique<BoolTypeHandler>());
        reg->registerType<uint>(std::make_unique<UIntTypeHandler>());
        reg->registerType<int>(std::make_unique<IntTypeHandler>());
        reg->registerType<float>(std::make_unique<FloatTypeHandler>());
        reg->registerType<double>(std::make_unique<DoubleTypeHandler>());
        reg->registerType<char>(std::make_unique<CharTypeHandler>());
        reg->registerType<uchar>(std::make_unique<UCharTypeHandler>());
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
        window = Window(tWinWidth, tWinHeight);
        if (!window.initialize(tTitle, tWinState)) return -1;
        //Select renderer.
        if (!onRendererSelect()) return -2;
#ifndef FS_NO_REG_DEFAULT_SHADER
        Shader::setDefaultShader(Embedded::defaultShaderVert, Embedded::defaultShaderFrag);
        enviroment().renderer->setPrimitiveShader(Embedded::primitiveShaderVert, Embedded::primitiveShaderFrag);
#endif // !FS_NO_REG_DEFAULT_SHADER
        //Final steps.
        CONFIG::checkGlobalFile();
        onInitialize();
#ifdef FS_PRINT_DEBUG_MSGS
        LOG_DBG("Run initialize");
#endif // FS_PRINT_DEBUG_MSGS
        //Update loop.
        LOG_STATE("UPDATE LOOP");
        while (window.isOpen()) {
#ifdef FS_INCLUDE_NVTX
            nvtx3::scoped_range u{ "update" };
#endif // FS_INCLUDE_NVTX
            window.pollEvents();
            //Per-frame time logic.
            double currentFrame = glfwGetTime();
            DeltaTime::sInstance()->set(static_cast<float>(currentFrame - mLastFrame));
            mLastFrame = currentFrame;
            mFrameCount++;
            if (currentFrame - mLastFrameFPS >= 1.0) {
                DeltaTime::sInstance()->setFps(mFrameCount);
                mFrameCount = 0;
                mLastFrameFPS = currentFrame;
            }
            if (window.isMinimized()) continue;
            window.clearBuffers();
            if (updateViewport) enviroment().renderer->setViewportSize(window.getSize());
            //ImGui & DevView window.
            if((Keyboard::getKey(KeyCode::LEFT_CONTROL) || Keyboard::getKey(KeyCode::RIGHT_CONTROL))
                && Keyboard::keyDown(KeyCode::SLASH)) DEVVIEW::sDrawDevView = true;
#ifdef FS_INCLUDE_NVTX
            nvtx3::scoped_range r{ "app update & imgui" };
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
        if (Log::sSaveLogs) Log::destroyFileLogger();
        return 0;
    }
    void App::onPreInitialize() {}
    bool App::onRendererSelect() {
        //Check for Vulkan.
        bool isVulkan = (glfwVulkanSupported() == 1);
#ifdef FS_PRINT_DEBUG_MSGS
        LOGF_DBG("Vulkan is%s supported on current machine.", isVulkan ? "" : "n't");
#endif // FS_PRINT_DEBUG_MSGS
        //Renderer init.
        enviroment().renderer = std::make_unique<Renderer>();
#ifdef FS_RENDERER_OGL
        enviroment().renderer = std::make_unique<OGLRenderer>();
#endif // FS_RENDERER_OGL
        if (!enviroment().renderer->initialize()) return false;
        enviroment().renderer->loadExtencions();
        enviroment().renderer->printInfo();
        enviroment().renderer->initializeParams();
        enviroment().renderer->imguiInitialize(window.ptr());
        return true;
    }
    void App::onInitialize() {}
    void App::onUpdate() {}
    void App::onShutdown() {}
}