#include <iostream>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

#include "XEngine/App.hpp"
#include "XEngine/Event.hpp"
#include "XEngine/Window.hpp"
#include "XEngine/Log.hpp"
#include "XEngine/UI/TUI.hpp"
#include "XEngine/Rendering/OpenGL/ShaderProgram.hpp"
#include "XEngine/Rendering/OpenGL/VertexBuffer.hpp"
#include "XEngine/Rendering/OpenGL/IndexBuffer.hpp"
#include "XEngine/Rendering/OpenGL/VertexArray.hpp"
#include "XEngine/Rendering/OpenGL/Renderer.hpp"
#include "XEngine/Camera.hpp"

namespace XEngine {
	
    using namespace XEngine::OpenGL;
    using namespace XEngine::UI;

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
           uniform mat4 view_proj_matrix;
           out vec3 color;
           void main() {
              color = vertex_color;
              gl_Position = view_proj_matrix * model_matrix * vec4(vertex_position, 1.0);
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

	/// <summary>
	/// Occures at app startup (instantiation).
	/// </summary>
	App::App() {
        LOG_INFO("Initializing XEngine Core App.");
    }

	/// <summary>
	/// Occures on app shutdown (destruction).
	/// </summary>
	App::~App() {
		LOG_INFO("Shuting down XEngine Core App.");
	}

	/// <summary>
	/// Creates new instance of window for application.
	/// </summary>
	/// <param name="win_width">Width of the window.</param>
	/// <param name="win_height">Height of the window.</param>
	/// <param name="title">Name of the window.</param>
	/// <returns>Exit code. Only 0 is success.</returns>
	int App::start(unsigned int win_width, unsigned int win_height, const char* title) {

		//Create window pointer.
		mainWindow = std::make_unique<Window>(title, win_width, win_height);
		//Add event listeners.
		eventDispatcher.addEventListener<EventWindowClose>([&](EventWindowClose& event) {
			closeWindow = true;
		});
		//Apply them.
		mainWindow->setEventCallback([&](BaseEvent& e) {
			eventDispatcher.dispatch(e);
		});

#pragma region TriangleInstance
        //Instance all data to draw triangle.
        // TODO: Move to other class.
        //Create shader program.
        shaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
        if (!shaderProgram->isCompilied()) {
            LOG_CRIT("Error while compiling main shader.");
            return -110;
        }
        //Vertex buffers and array.
        BufferLayout bufLayout{
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };
        vao = std::make_unique<VertexArray>();
        positions_colorsVBO = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), bufLayout);
        indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
        //Add buffers to array.
        vao->addVertexBuffer(*positions_colorsVBO);
        vao->setIndexBuffer(*indexBuffer);
#pragma endregion

        //Update cycle.
		while (!closeWindow) {

            //Clear color buffer.
            Renderer::setClearColorRGB(bgColor[0], bgColor[1], bgColor[2]);
            Renderer::clear();

#pragma region TriangleUpdate
            // TODO: Move to other class.
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
            //Camera.
            camera.setPositionRotation(glm::vec3(camPosition[0], camPosition[1], camPosition[2]),
                glm::vec3(camRotation[0], camRotation[1], camRotation[2]));
            camera.setProjectionMode(camIsPresp ? Camera::ProjectionMode::Perspective :
                Camera::ProjectionMode::Ortographic);
            shaderProgram->setMatrix4("view_proj_matrix",
                camera.getProjectionMatrix() * camera.getViewMatrix());
            //Render.
            Renderer::draw(*vao);
#pragma endregion

            //Create new frame for ImGui.
            TUI::update();
            bool eADSshow = true;
            TUI::ShowExampleAppDockSpace(&eADSshow);
            //Draw ImGui.
            onUIDraw();
            //Render ImGui.
            TUI::draw();

			mainWindow->update();
			update();
		}
		mainWindow = nullptr;

        return 0;

	}

}