#ifndef FS_APP_H
#define FS_APP_H

#include "common.hpp"
#include "window.hpp"
#include "renderer.hpp"

namespace Firesteel {

	class App {
    private:
        double mLastFrameFPS = 0, mLastFrame = 0;
        int mFrameCount = 0;
	public:
		App() {
			LOG_INFO("Initializing Firesteel App.");
		}
		virtual ~App() { }
		void shutdown() {
			window.close();
		}
		virtual int start(const char* tTitle = "Firesteel App",
            const unsigned int tWinWidth = 800, const unsigned int tWinHeight = 600, const WindowState tWinState = WS_NORMAL) {
            LOG("Firesteel 0.2.0.6");
            LOG_C("[-   Dev branch  -]", CMD_F_PURPLE);
            LOG_STATE("STARTUP");
			onPreInitialize();
            //Create window.
            window = Window(tWinWidth, tWinHeight);
            if(!window.initialize(tTitle, tWinState))
                return 1;
            //Check for Vulkan.
            bool isVulkan = (glfwVulkanSupported() == 1);
            LOG_INFO(isVulkan ? "Vulkan is supported on current machine."
                : "Vulkan isn't supported on current machine.");
            //Renderer init.
            Renderer r = Renderer();
            if (!r.initialize()) return -1;
            r.loadExtencions();
            r.printInfo();
            r.initializeParams();
            onInitialize();

            //Update loop.
            LOG_STATE("UPDATE LOOP");
            while (window.isOpen()) {
                window.pollEvents();
                //Per-frame time logic.
                double currentFrame = glfwGetTime();
                deltaTime = static_cast<float>(currentFrame - mLastFrame);
                mLastFrame = currentFrame;
                mFrameCount++;
                if (currentFrame - mLastFrameFPS >= 1.0) {
                    fps = mFrameCount;
                    mFrameCount = 0;
                    mLastFrameFPS = currentFrame;
                }
                window.clearBuffers();
                onUpdate();
                window.swapBuffers();
            }
            LOG_STATE("SHUTDOWN");
            onShutdown();
            //Quitting.
            window.terminate();
            LOG_INFO("Window terminated");
            LOG_STATE("QUIT");
            return 0;
		}
        // (overridable)
        // Runs before any initialization.
		virtual void onPreInitialize() { }
        // (overridable)
        // Runs after window and renderer initialization.
		virtual void onInitialize() { }
        // (overridable)
        // Runs every frame.
		virtual void onUpdate() { }
        // (overridable)
        // Runs after the window is closed or on window.close()/app.shutdown().
		virtual void onShutdown() { }

		Window window;
		unsigned int fps = 0;
        float deltaTime = 0.0f;
	};

}

#endif // !FS_APP_H
