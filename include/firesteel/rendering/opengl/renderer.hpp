#ifdef FS_RENDERER_OGL
#ifndef FS_OGL_RENDERER_H
#define FS_OGL_RENDERER_H
#include <firesteel/renderer.hpp>
#include <firesteel/rendering/opengl/mesh.hpp>
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
		bool initialize() override {
            //GLAD (OpenGL) init.
            mInitialized=(gladLoadGL(glfwGetProcAddress) != 0);
            if(mInitialized) { LOG_INFO("OpenGL initialized successfully"); }
            else LOG_CRIT("Failed to initialize OpenGL");
            return mInitialized;
		}
        void initializeParams() override {
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
        void printInfo() override {
            //OpenGL info.
            LOG_INFO("OpenGL context:");
            LOG_INFO(std::string("	Vendor: ") + reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            LOG_INFO(std::string("	Renderer: ") + reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            LOG_INFO(std::string("	Version: ") + reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        }
		void loadExtencions() override {
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
        void clearBuffers(const glm::vec3& tColor) override {
            glClearColor(static_cast<GLfloat>(tColor[0]), static_cast<GLfloat>(tColor[1]),
                static_cast<GLfloat>(tColor[2]), static_cast<GLfloat>(1.0f));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }
        void setAlphaBlending(const bool& tVal) override {
            glBlendFunc(GL_SRC_ALPHA, tVal?GL_ONE_MINUS_SRC_ALPHA:GL_ONE);
        }
        void setDrawMode(const DrawMode& tDrawMode) override {
            switch (tDrawMode) {
            case DrawMode::DM_WIRE:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            default:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            }
        }
        void setDepthTestType(const DepthTestType& tVal) override {
            uint v=0;
            switch (tVal) {
            case DTT_LESS:
                v=GL_LESS;
                break;
            case DTT_LEQUAL:
                v=GL_LEQUAL;
                break;
            case DTT_EQUAL:
                v=GL_EQUAL;
                break;
            case DTT_GEQUAL:
                v=GL_GEQUAL;
                break;
            case DTT_GREATER:
                v=GL_GREATER;
                break;
            case DTT_NOT_EQUAL:
                v=GL_NOTEQUAL;
                break;
            default:
                v=GL_ALWAYS;
                break;
            }
            glDepthFunc(v);
        }
        void setViewportSize(const int& tX, const int& tY) override {
            glViewport(0, 0, static_cast<GLsizei>(tX), static_cast<GLsizei>(tY));
        }
        void imguiInitialize(GLFWwindow* tWin) override {
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
        void imguiNewFrame() override {
#if FS_CONTEXT_MAJOR > 2
            ImGui_ImplOpenGL3_NewFrame();
#else
            ImGui_ImplOpenGL2_NewFrame();
#endif
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }
        void imguiRender(GLFWwindow* tWin) override {
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
        void imguiShutdown() {
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

        void drawPoint(const glm::vec2& tPos,const glm::vec3& tColor=glm::vec3(1),const float& tSize=10.f) override {
            glColor3f(tColor.r,tColor.g,tColor.b);
            glPointSize(tSize);
            glBegin(GL_POINTS);
            glVertex2f(tPos.x,tPos.y);
            glEnd();
            glFlush();
        }
        void drawPoint(const glm::vec3& tPos,const glm::vec3& tColor=glm::vec3(1),const float& tSize=10.f) override {
            glColor3f(tColor.r,tColor.g,tColor.b);
            glPointSize(tSize);
            glBegin(GL_POINTS);
            glVertex3f(tPos.x,tPos.y,tPos.z);
            glEnd();
            glFlush();
        }
        void drawLine(const std::vector<glm::vec3>& tPosList,const glm::vec3& tColor=glm::vec3(1)) override {
            glColor3f(tColor.r,tColor.g,tColor.b);
            glBegin(GL_LINES);
            for(uint i=0;i<tPosList.size();i++)
                glVertex3f(tPosList[i].x,tPosList[i].y,tPosList[i].z);
            glEnd();
            glFlush();
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
        std::unique_ptr<Mesh> createMesh(const std::vector<Vertex>& tVertices, const std::vector<uint>& tIndices, Material* tMaterial) override {
            return std::make_unique<OGLMesh>(tVertices, tIndices, tMaterial);
        }
	};

}

#endif // !FS_OGL_RENDERER_H
#endif // FS_RENDERER_OGL