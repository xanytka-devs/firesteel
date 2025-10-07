#ifdef FS_RENDERER_OGL
#ifndef FS_OGL_RENDERER_H
#define FS_OGL_RENDERER_H
#include "../renderer.hpp"
#if FS_CONTEXT_MAJOR > 2
#include <backends/imgui_impl_opengl3.h>
#else
#include <backends/imgui_impl_opengl2.h>
#endif
#include <backends/imgui_impl_glfw.h>

namespace Firesteel {
	class OGLRenderer : public Renderer {
    public:
        OGLRenderer() {}
		virtual bool initialize() override {
            //GLAD (OpenGL) init.
            mInitialized=(gladLoadGL(glfwGetProcAddress) != 0);
            if(mInitialized) { LOG_INFO("OpenGL initialized successfully"); }
            else LOG_CRIT("Failed to initialize OpenGL");
            return mInitialized;
		}
        virtual void initializeParams() override {
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
        virtual void printInfo() override {
            //OpenGL info.
            LOG_INFO("OpenGL context:");
            LOG_INFO(std::string("	Vendor: ") + reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            LOG_INFO(std::string("	Renderer: ") + reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            LOG_INFO(std::string("	Version: ") + reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        }
		virtual void loadExtencions() override {
            if(!glfwExtensionSupported("GL_ARB_debug_output")) LOG_WARN("General debug output isn't supported.");
            if(!glfwExtensionSupported("GL_AMD_debug_output")) LOG_WARN("AMD debug output isn't supported.");
            int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
            if(flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
                //Initialize debug output.
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#if (FS_CONTEXT_MAJOR >= 4) && (FS_CONTEXT_MINOR >= 3)
                //Set custom debug output.
                glDebugMessageCallback(glDebugOutput, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
            }
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Loaded OpenGL extensions");
#endif // FS_PRINT_DEBUG_MSGS
		}
        virtual void clearBuffers(const glm::vec3& tColor) override {
            glClearColor(static_cast<GLfloat>(tColor[0]), static_cast<GLfloat>(tColor[1]),
                static_cast<GLfloat>(tColor[2]), static_cast<GLfloat>(1.0f));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }
        virtual void setViewportSize(const int& tX, const int& tY) override {
            glViewport(0, 0, static_cast<GLsizei>(tX), static_cast<GLsizei>(tY));
        }
        virtual void setViewportSize(const glm::vec2& tSize) override {
            setViewportSize(static_cast<int>(tSize.x), static_cast<int>(tSize.y));
        }
        virtual void setDrawMode(const DrawMode& tDrawMode) override {
            switch (tDrawMode) {
            case DrawMode::DM_WIRE:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            default:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            }
        }
        virtual void imguiInitialize(GLFWwindow* tWin) override {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io=ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard
                | ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;
            //Setup Dear ImGui style.
            ImGui::StyleColorsDark();
            //Choose backend.
            ImGui_ImplGlfw_InitForOpenGL(tWin, true);
#if FS_CONTEXT_MAJOR > 2
            ImGui_ImplOpenGL3_Init();
#else
            ImGui_ImplOpenGL2_Init();
#endif
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Initialized ImGui");
#endif // FS_PRINT_DEBUG_MSGS
        }
        virtual void imguiNewFrame() override {
#if FS_CONTEXT_MAJOR > 2
            ImGui_ImplOpenGL3_NewFrame();
#else
            ImGui_ImplOpenGL2_NewFrame();
#endif
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }
        virtual void imguiRender(GLFWwindow* tWin) override {
#ifdef FS_INCLUDE_NVTX
                nvtx3::scoped_range d{"imgui draw"};
#endif // FS_INCLUDE_NVTX
            ImGui::Render();
#if FS_CONTEXT_MAJOR > 2
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#else
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
#endif
            //Viewports need persistent context updates.
            if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
#ifdef FS_INCLUDE_NVTX
                nvtx3::mark("reset context");
#endif // FS_INCLUDE_NVTX
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(tWin);
            }
        }
        virtual void imguiShutdown() {
#if FS_CONTEXT_MAJOR > 2
            ImGui_ImplOpenGL3_Shutdown();
#else
            ImGui_ImplOpenGL2_Shutdown();
#endif
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
#ifdef FS_PRINT_DEBUG_MSGS
            LOG_DBG("Shutdown ImGui");
#endif // FS_PRINT_DEBUG_MSGS
        }
    private:
        static void APIENTRY glDebugOutput(GLenum tSource,
            GLenum tType,
            unsigned int tID,
            GLenum tSeverity,
            GLsizei tLength,
            const char* tMsg,
            const void* tParam) {
            //Ignore non-significant error/warning codes.
            if (tID == 131169 || tID == 131185 || tID == 131218 || tID == 131204) return;

            LOGF_C("OpenGL Error(%d): ", true, CMD_F_RED, CMD_BG_BLACK, std::to_string(tID));
            LOG(tMsg);

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
            LOG("-----", "\n", CMD_F_RED);
        }
	};

}

#endif // !FS_OGL_RENDERER_H
#endif // FS_RENDERER_OGL