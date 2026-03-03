#ifndef FS_RENDERER_H
#define FS_RENDERER_H

#include <imgui.h>
#include <firesteel/common.hpp>
#include <firesteel/rendering/mesh.hpp>

namespace Firesteel {
	class Renderer {
    public:
        enum DrawMode {
            DM_FILL=0,
            DM_WIRE=1
        };
        enum DepthTestType {
            DTT_LESS=0,
            DTT_LEQUAL=1,
            DTT_EQUAL=2,
            DTT_GEQUAL=3,
            DTT_GREATER=4,
            DTT_NOT_EQUAL=5,
            DTT_ALWAYS=6
        };
        Renderer() { }
		virtual bool initialize() { return true; }
        virtual void initializeParams() { }
        virtual void printInfo() { LOG_INFO("Headless context"); }
        virtual void loadExtencions() {}

        virtual void clearBuffers(const glm::vec3& tColor) { }

        virtual void setAlphaBlending(const bool& tVal) { }
        virtual void setDrawMode(const DrawMode& tDrawMode) {}
        virtual void setDepthTestType(const DepthTestType& tVal) { }

        virtual void setViewportSize(const int& tX, const int& tY) { }
        void setViewportSize(const glm::vec2& tSize) {
            setViewportSize(static_cast<int>(tSize.x), static_cast<int>(tSize.y));
        }

        virtual void imguiInitialize(GLFWwindow* tWin) { }
        virtual void imguiNewFrame() {}
        virtual void imguiRender(GLFWwindow* tWin) { }
        virtual void imguiShutdown() { }

        virtual std::unique_ptr<Mesh> createMesh(const std::vector<Vertex>& tVertices, const std::vector<uint>& tIndices, Material* tMaterial) {
            return std::make_unique<Mesh>(tVertices,tIndices,tMaterial);
        }
    protected:
        bool mInitialized=false;
	};

}

#endif // !FS_RENDERER_H
