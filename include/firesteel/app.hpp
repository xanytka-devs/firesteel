#ifndef FS_APP_H
#define FS_APP_H

#include <firesteel/common.hpp>
#include <firesteel/window.hpp>
#include <firesteel/enviroment.hpp>

namespace Firesteel {
	class App {
    private:
        double mLastFrameFPS=0, mLastFrame=0;
        int mFrameCount=0;
	public:
        App();
		virtual ~App() { }
        void shutdown();
        // [!DANGEROUS]
        // Immediately errors out of application no matter the cost.
        // This means no `onShutdown()` will be run.
        void critShutdown(const int& tErrorCode = -13,
            const char* tError = "Critical shutdown was called");
        virtual int start(const char* tTitle = "Firesteel App",
            const uint tWinWidth = 800, const uint tWinHeight = 600, const WindowState tWinState = WS_NORMAL);
        // [!OVERRIDABLE]
        // Runs before any initialization.
        virtual void onPreInitialize();
        // [!OVERRIDABLE]
        // Runs after preinitialize. Used to select renderer.
        virtual bool onRendererSelect();
        // [!OVERRIDABLE]
        // Runs after window and renderer initialization.
        virtual void onInitialize();
        // [!OVERRIDABLE]
        // Runs every frame.
        virtual void onUpdate();
        // [!OVERRIDABLE]
        // Runs after the window is closed or on window.close()/app.shutdown().
        virtual void onShutdown();

        Enviroment& enviroment() { return *(Enviroment::sInstance()); }
		Window window;
        bool updateViewport=true;
	};
}

#endif // !FS_APP_H
