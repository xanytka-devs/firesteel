#ifndef FS_RENDERER_H
#define FS_RENDERER_H
#include "common.hpp"
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace Firesteel {
	class Renderer {
    public:
        Renderer() { }
        // Basic renderer initialization.
		bool initialize() {
            //GLAD (OpenGL) init.
            mInitialized=(gladLoadGL(glfwGetProcAddress) != 0);
#ifdef FS_PRINT_DEBUG_MSGS
            if(mInitialized) { LOG_DBG("OpenGL initialized successfully"); }
            else LOG_DBG("Failed to initialize OpenGL");
#endif // FS_PRINT_DEBUG_MSGS
            return mInitialized;
		}
        // Sets runtime parameters for renderer.
        void initializeParams() {
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_STENCIL_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glEnable(GL_FRAMEBUFFER_SRGB);
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Set OpenGL variables");
#endif // FS_PRINT_DEBUG_MSGS
        }
        // Prints info about renderer installation.
        void printInfo() {
            //OpenGL info.
            LOG_INFO("OpenGL context:");
            LOG_INFO(std::string("	Vendor: ") + reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            LOG_INFO(std::string("	Renderer: ") + reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            LOG_INFO(std::string("	Version: ") + reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        }
        // Loads needed renderer extencions.
		void loadExtencions() {
            if(!glfwExtensionSupported("GL_ARB_debug_output")) LOG_WARN("General debug output isn't supported.");
            if(!glfwExtensionSupported("GL_AMD_debug_output")) LOG_WARN("AMD debug output isn't supported.");
            int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
            if(flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
                //Initialize debug output.
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                //For OpenGL 4.x.
                glDebugMessageCallback(glDebugOutput, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Loaded OpenGL extensions");
#endif // FS_PRINT_DEBUG_MSGS
		}
        // Initializes ImGui.
        void initializeImGui(GLFWwindow* tWin) {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io=ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard
                | ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;
            // Setup Dear ImGui style
            ImGui::StyleColorsDark(); // you can also use ImGui::StyleColorsClassic();
            // Choose backend
            ImGui_ImplGlfw_InitForOpenGL(tWin, true);
            ImGui_ImplOpenGL3_Init();
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Initialized ImGui");
#endif // FS_PRINT_DEBUG_MSGS
        }
        // Creates new frame for ImGui (before any Begin-s).
        void newFrameImGui() {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }
        // Draws all ImGui data.
        void renderImGui(GLFWwindow* tWin) {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(tWin);
            }
        }
        // Destroys ImGui context.
        void shutdownImGui() {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Shutdown ImGui");
#endif // FS_PRINT_DEBUG_MSGS
        }
    private:
        bool mInitialized=false;

        static void APIENTRY glDebugOutput(GLenum tSource,
            GLenum tType,
            unsigned int tID,
            GLenum tSeverity,
            GLsizei tLength,
            const char* tMsg,
            const void* tParam) {
            //Ignore non-significant error/warning codes.
            if (tID == 131169 || tID == 131185 || tID == 131218 || tID == 131204) return;

            LOG_C("OpenGL Error(" + std::to_string(tID) + "): ", CMD_F_RED, "");
            LOG_C(tMsg);

            switch (tSource) {
            case GL_DEBUG_SOURCE_API:             LOG("Source: API"); break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   LOG("Source: Window System"); break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: LOG("Source: Shader Compiler"); break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     LOG("Source: Third Party"); break;
            case GL_DEBUG_SOURCE_APPLICATION:     LOG("Source: Application"); break;
            case GL_DEBUG_SOURCE_OTHER:           LOG("Source: Other"); break;
            }

            switch (tType) {
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

            switch (tSeverity) {
            case GL_DEBUG_SEVERITY_HIGH:         LOG("Severity: high"); break;
            case GL_DEBUG_SEVERITY_MEDIUM:       LOG("Severity: medium"); break;
            case GL_DEBUG_SEVERITY_LOW:          LOG("Severity: low"); break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: LOG("Severity: notification"); break;
            }
            LOG_C("-----", CMD_F_RED);
        }
	};

}

#endif // !FS_RENDERER_H
