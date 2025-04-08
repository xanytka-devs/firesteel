#ifndef FS_RENDERER_H
#define FS_RENDERER_H

#include "common.hpp"

namespace Firesteel {

#define BOUND_GL_VERSION_MAJOR 4
#define BOUND_GL_VERSION_MINOR 3

	class Renderer {
    public:
        Renderer() {

        }
		bool initialize() {
            // GLAD (OpenGL) init.
            mInitialized = (gladLoadGL(glfwGetProcAddress) != 0);
            return mInitialized;
		}
        void initializeParams() {
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_STENCIL_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glEnable(GL_FRAMEBUFFER_SRGB);
        }
        void printInfo() {
            // OpenGL info.
            LOG_INFO("OpenGL context:");
            LOG_INFO(std::string("	Vendor: ") + reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            LOG_INFO(std::string("	Renderer: ") + reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            LOG_INFO(std::string("	Version: ") + reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        }
		void loadExtencions() {
            if (!glfwExtensionSupported("GL_ARB_debug_output")) LOG_WARN("General debug output isn't supported.");
            if (!glfwExtensionSupported("GL_AMD_debug_output")) LOG_WARN("AMD debug output isn't supported.");
            if (!glfwExtensionSupported("GL_ARB_direct_state_access")) LOG_WARN("Official DSA isn't supported.");
            if (!glfwExtensionSupported("GL_EXT_direct_state_access")) LOG_WARN("Unofficial DSA isn't supported.");
            int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
            if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
                // initialize debug output 
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                if (BOUND_GL_VERSION_MAJOR >= 4) {
                    glDebugMessageCallback(glDebugOutput, nullptr);
                    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
                }
            }
		}
    private:
        bool mInitialized = false;

        static void APIENTRY glDebugOutput(GLenum source,
            GLenum type,
            unsigned int id,
            GLenum severity,
            GLsizei length,
            const char* message,
            const void* userParam)
        {
            // ignore non-significant error/warning codes
            if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

            LOG("---------------");
            LOG("Debug message (" + std::to_string(id) + "): " + message);

            switch (source) {
            case GL_DEBUG_SOURCE_API:             LOG("Source: API"); break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   LOG("Source: Window System"); break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: LOG("Source: Shader Compiler"); break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     LOG("Source: Third Party"); break;
            case GL_DEBUG_SOURCE_APPLICATION:     LOG("Source: Application"); break;
            case GL_DEBUG_SOURCE_OTHER:           LOG("Source: Other"); break;
            }
            switch (type) {
            case GL_DEBUG_TYPE_ERROR:               LOG("Type: Error"); break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: LOG("Type: Deprecated Behaviour"); break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  LOG("Type: Undefined Behaviour"); break;
            case GL_DEBUG_TYPE_PORTABILITY:         LOG("Type: Portability"); break;
            case GL_DEBUG_TYPE_PERFORMANCE:         LOG("Type: Performance"); break;
            case GL_DEBUG_TYPE_MARKER:              LOG("Type: Marker"); break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          LOG("Type: Push Group"); break;
            case GL_DEBUG_TYPE_POP_GROUP:           LOG("Type: Pop Group"); break;
            case GL_DEBUG_TYPE_OTHER:               LOG("Type: Other"); break;
            }

            switch (severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:         LOG("Severity: high"); break;
            case GL_DEBUG_SEVERITY_MEDIUM:       LOG("Severity: medium"); break;
            case GL_DEBUG_SEVERITY_LOW:          LOG("Severity: low"); break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: LOG("Severity: notification"); break;
            }
            LOG("---------------");
        }
	};

}

#endif // !FS_RENDERER_H
