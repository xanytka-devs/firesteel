#ifndef FS_APP_H
#define FS_APP_H

#include "common.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "internal/config.hpp"
#include "internal/devview.hpp"

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
            //Versioning.
            LOG("Firesteel 0.2.1");
            LOG_STATE("STARTUP");
            //Do some randomization.
            __time64_t long_time;
	        _time64(&long_time);
	        srand(static_cast<unsigned int>(long_time));
            //Preinitialize.
			onPreInitialize();
            //Create a window.
            window = Window(tWinWidth, tWinHeight);
            if(!window.initialize(tTitle, tWinState))
                return 1;
            //Check for Vulkan.
            bool isVulkan = (glfwVulkanSupported() == 1);
            LOG_INFO(std::string("Vulkan is") + (isVulkan ? "" : "n't") + " supported on current machine.");
            //Renderer init.
            Renderer r = Renderer();
            if (!r.initialize()) return -1;
            r.loadExtencions();
            r.printInfo();
            r.initializeParams();
            r.initializeImGui(window.ptr());
            //Final prep before update loop.
            CONFIG::checkGlobalFile();
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
                if(window.isMinimized()) continue;
                window.clearBuffers();
                //ImGui & DevView window.
                if((Keyboard::getKey(KeyCode::LEFT_CONTROL) || Keyboard::getKey(KeyCode::RIGHT_CONTROL))
                    && Keyboard::keyDown(KeyCode::SLASH)) DEVVIEW::drawDevView = true;
                r.newFrameImGui();
                //Call virtual update.
                onUpdate();
                //Draw DevView and finalize update.
                DEVVIEW::draw(deltaTime, fps);
                r.renderImGui(window.ptr());
                window.swapBuffers();
            }
            //Shutdown.
            LOG_STATE("SHUTDOWN");
            onShutdown();
            r.shutdownImGui();
            //Quitting.
            window.close();
            glfwTerminate();
            LOG_INFO("Window terminated");
            LOG_STATE("QUIT");
#ifdef FS_DUMP_LOGS
            Log::destroyFileLogger();
#endif //!FS_DUMP_LOGS
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
