#ifndef FS_RENDERER_H
#define FS_RENDERER_H

#include <imgui.h>
#include "common.hpp"

namespace Firesteel {
	class Renderer {
    public:
        enum DrawMode {
            DM_FILL=0,
            DM_WIRE=1
        };
        Renderer() { }
		virtual bool initialize() { return true; }
        virtual void initializeParams() { }
        virtual void printInfo() { LOG_INFO("Headless context"); }
        virtual void loadExtencions() {}
        virtual void clearBuffers(const glm::vec3& tColor) { }
        virtual void setViewportSize(const int& tX, const int& tY) { }
        virtual void setViewportSize(const glm::vec2& tSize) { }
        virtual void setDrawMode(const DrawMode& tDrawMode) { }
        virtual void imguiInitialize(GLFWwindow* tWin) { }
        virtual void imguiNewFrame() {}
        virtual void imguiRender(GLFWwindow* tWin) { }
        virtual void imguiShutdown() { }
    protected:
        bool mInitialized=false;
	};

}

#endif // !FS_RENDERER_H
