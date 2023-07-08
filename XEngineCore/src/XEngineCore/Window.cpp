#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "XEngineCore/Event.hpp"
#include "XEngineCore/Window.hpp"
#include "XEngineCore/Log.hpp"
#include "XEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "XEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "XEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "XEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

namespace XEngine {

    static bool glfwInitialized = false;

    GLfloat positions_colors[] = {
        -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 2, 3, 2, 1
    };

    const char* vertexShader =
        R"(#version 460
           layout(location = 0) in vec3 vertex_position;
           layout(location = 1) in vec3 vertex_color;
           uniform mat4 model_matrix;
           out vec3 color;
           void main() {
              color = vertex_color;
              gl_Position = model_matrix * vec4(vertex_position, 1.0);
           })";

    const char* fragmentShader =
        R"(#version 460
           in vec3 color;
           out vec4 frag_color;
           void main() {
              frag_color = vec4(color, 1.0);
           })";

    std::unique_ptr<ShaderProgram> shaderProgram;
    std::unique_ptr<VertexBuffer> positions_colorsVBO;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::unique_ptr<VertexArray> vao;
    float position[3] = { 0.f, 0.f, 0.f };
    float rotation[3] = { 0.f, 0.f, 0.f };
    float scale[3] = { 1.f, 1.f, 1.f };

	Window::Window(std::string title, const unsigned int width, const unsigned int height)
		: w_data({std::move(title), width, height}) {
        //Initialize basis.
		int result = initialize();
        //Initialize ImGui.
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
	}
	Window::~Window() {
		shutdown();
	}

    int Window::initialize() {

        LOG_INFO("Creating instance of window '{0}', with size of {1}x{2}.", w_data.title, w_data.width, w_data.height);

        //Initialize the library.
        if (!glfwInitialized) {
            if (!glfwInit()) {
                LOG_CRIT("Error while trying to initialize GLFW (OpenGL).");
                return -100;
            }
            glfwInitialized = true;
        }

        //Create a windowed mode window and its OpenGL context.
        window = glfwCreateWindow(w_data.width, w_data.height, w_data.title.c_str(), nullptr, nullptr);
        if (!window) {
            LOG_CRIT("Error while trying to initialize window '{0}'.", w_data.title);
            glfwTerminate();
            return -101;
        }

        //Make the window's context current.
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRIT("Failed to initialize GLAD (OpenGL).");
            return -102;
        }

        //Handel events.
        glfwSetWindowUserPointer(window, &w_data);
        glfwSetWindowSizeCallback(window, [](GLFWwindow* pWindow, int width, int height) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            data.width = width;
            data.height = height;
            EventWindowResize event(width, height);
            data.eventCallbackFn(event);
        });
        glfwSetCursorPosCallback(window, [](GLFWwindow* pWindow, double x, double y) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            EventMouseMove event(x, y);
            data.eventCallbackFn(event);
        });
        glfwSetWindowCloseCallback(window, [](GLFWwindow* pWindow) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            EventWindowClose event;
            data.eventCallbackFn(event);
            LOG_INFO("Window '{0}' closed.", data.title);
        });

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* pWindow, int width, int height) {
            glViewport(0, 0, width, height);
        });

        //Instance all data to draw triangle.
        //Create shader program.
        shaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
        if (!shaderProgram->isCompilied()) {
            LOG_CRIT("Error while compiling main shader.");
            return -110;
        }
        //Vertex buffers and array.
        BufferLayout bufLayout {
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };
        vao = std::make_unique<VertexArray>();
        positions_colorsVBO = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), bufLayout);
        indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
        //Add buffers to array.
        vao->addVertexBuffer(*positions_colorsVBO);
        vao->setIndexBuffer(*indexBuffer);
        return 0;

	}
	void Window::shutdown() {
        glfwDestroyWindow(window);
        glfwTerminate();
	}
	void Window::update() {

        //Clear color buffer.
        glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        //Update ImGui values.
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(getWidth());
        io.DisplaySize.y = static_cast<float>(getHeight());
        //Create new frame for ImGui.
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //Draw ImGui.
        static bool drawDemoWindow = false;
        //Draw Demo window.
        if (drawDemoWindow) ImGui::ShowDemoWindow();
        //XEngine Testing Window.
        ImGui::Begin("XEngine Testing Window");
        ImGui::Text("General Testing");
        ImGui::ColorEdit4("Background Color", bgColor);
        ImGui::Checkbox("Draw Demo window", &drawDemoWindow);
        ImGui::Text("Matrix Operations");
        ImGui::SliderFloat3("Position", position, -1.f, 1.f);
        ImGui::SliderFloat3("Rotation", rotation, 0.f, 360.f);
        ImGui::SliderFloat3("Scale", scale, 0.f, 2.f);

        //Render triangle.
        shaderProgram->bind();
        //Scale matrix.
        glm::mat4 scaleMatrix(scale[0], 0, 0, 0, 0, scale[1], 0, 0, 0, 0, scale[2], 0, 0, 0, 0, 1);
        //Rotate matrix.
        float rIRX = glm::radians(rotation[0]);
        glm::mat4 rotationXMatrix(1, 0, 0, 0, 0, cos(rIRX), -sin(rIRX), 0,
            0, sin(rIRX), cos(rIRX), 0, 0, 0, 0, 1);
        float rIRY = glm::radians(rotation[1]);
        glm::mat4 rotationYMatrix(cos(rIRY), 0, sin(rIRY), 0, 0, 1, 0, 0,
            -sin(rIRY), 0, cos(rIRY), 0, 0, 0, 0, 1);
        float rIRZ = glm::radians(rotation[2]);
        glm::mat4 rotationZMatrix(cos(rIRZ), sin(rIRZ), 0, 0,
            -sin(rIRZ), cos(rIRZ), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        //Translate matrix.
        glm::mat4 positionMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
            position[0], position[1], position[2], 1);
        //Model matrix.
        glm::mat4 model_matrix = scaleMatrix * (rotationXMatrix * rotationYMatrix * rotationZMatrix) * positionMatrix;
        shaderProgram->setMatrix4("model_matrix", model_matrix);
        vao->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vao->getIndicesCount()), GL_UNSIGNED_INT, nullptr);
        ImGui::End();

        //Render ImGui.
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //Swap front and back buffers.
        glfwSwapBuffers(window);
        //Poll for and process events.
        glfwPollEvents();

	}

}