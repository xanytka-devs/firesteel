#ifndef FS_APP_H
#define FS_APP_H

#include "embedded.hpp"
#include "common.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "internal/devview.hpp"

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
		virtual int start(const char* tTitle="Firesteel App",
            const unsigned int tWinWidth=800, const unsigned int tWinHeight=600, const WindowState tWinState=WS_NORMAL) {
            LOG(std::string("Firesteel ") + FiresteelVersion);
            LOG_STATE("STARTUP");
			onPreInitialize();
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Run preinitialize");
#endif // FS_PRINT_DEBUG_MSGS
            //Create a window.
            window=Window(tWinWidth, tWinHeight);
            if(!window.initialize(tTitle, tWinState))
                return 1;
            //Check for Vulkan.
            bool isVulkan=(glfwVulkanSupported() == 1);
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG(std::string("Vulkan is") + (isVulkan ? "" : "n't") + " supported on current machine.");
#endif // FS_PRINT_DEBUG_MSGS
            //Renderer init.
            renderer=Renderer();
            if(!renderer.initialize()) return -1;
            renderer.loadExtencions();
            renderer.printInfo();
            renderer.initializeParams();
            renderer.imguiInitialize(window.ptr());
            Shader::setDefaultShader(Embedded::defaultShaderVert,Embedded::defaultShaderFrag);
            //Final steps.
            CONFIG::checkGlobalFile();
            onInitialize();
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Run initialize");
#endif // FS_PRINT_DEBUG_MSGS
            //Update loop.
            LOG_STATE("UPDATE LOOP");
            while (window.isOpen()) {
                window.pollEvents();
                //Per-frame time logic.
                double currentFrame=glfwGetTime();
                deltaTime=static_cast<float>(currentFrame - mLastFrame);
                mLastFrame=currentFrame;
                mFrameCount++;
                if(currentFrame - mLastFrameFPS >= 1.0) {
                    fps=mFrameCount;
                    mFrameCount=0;
                    mLastFrameFPS=currentFrame;
                }
                if(window.isMinimized()) continue;
                if(updateViewport) renderer.setViewportSize(window.getSize());
                window.clearBuffers();
                if((Keyboard::getKey(KeyCode::LEFT_CONTROL) || Keyboard::getKey(KeyCode::RIGHT_CONTROL))
                    && Keyboard::keyDown(KeyCode::SLASH)) DEVVIEW::sDrawDevView=true;
                renderer.imguiNewFrame();
                onUpdate();
                DEVVIEW::draw(deltaTime, fps);
                renderer.imguiRender(window.ptr());
                window.swapBuffers();
            }
            //Shutdown.
            LOG_STATE("SHUTDOWN");
            onShutdown();
            renderer.imguiShutdown();
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
        // Runs after window and renderer initialization.
		virtual void onInitialize() { }
        // [!OVERRIDABLE]
        // Runs every frame.
		virtual void onUpdate() { }
        // [!OVERRIDABLE]
        // Runs after the window is closed or on window.close()/app.shutdown().
		virtual void onShutdown() { }

        Renderer renderer;
		Window window;
        bool updateViewport=true;
		unsigned int fps=0;
        float deltaTime=0.0f;
	};
}

#endif // !FS_APP_H
