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
#include "XEngine/Rendering/OpenGL/Texture2D.hpp"
#include "XEngine/Camera.hpp"
#include "XEngine/Input.hpp"

namespace XEngine {
	
    using namespace XEngine::OpenGL;
    using namespace XEngine::UI;

    GLfloat cubeDataArray[] = {
        -1.f, -1.f, -1.f,   1.f, 0.f,
        -1.f,  1.f, -1.f,   0.f, 0.f,
        -1.f, -1.f,  1.f,   1.f, 1.f,
        -1.f,  1.f,  1.f,   0.f, 1.f,
         1.f, -1.f, -1.f,   1.f, 0.f,
         1.f,  1.f, -1.f,   0.f, 0.f,
         1.f, -1.f,  1.f,   1.f, 1.f,
         1.f,  1.f,  1.f,   0.f, 1.f
    };

    GLuint indices[] = {
        0, 1, 2, 3, 2, 1, //Front.
        4, 5, 6, 7, 6, 5, //Back.
        0, 4, 6, 0, 2, 6, //Right.
        1, 5, 3, 3, 7, 5, //Left.
        3, 7, 2, 7, 6, 2, //Top.
        1, 5, 0, 5, 0, 4  //Bottom.
    };

    void generateCircle(unsigned char* data,
        const unsigned int width,
        const unsigned int height,
        const unsigned int center_x,
        const unsigned int center_y,
        const unsigned int radius,
        const unsigned char color_r,
        const unsigned char color_g,
        const unsigned char color_b) {
        for (unsigned int x = 0; x < width; ++x) {
            for (unsigned int y = 0; y < height; ++y) {
                if ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) < radius * radius) {
                    data[3 * (x + width * y) + 0] = color_r;
                    data[3 * (x + width * y) + 1] = color_g;
                    data[3 * (x + width * y) + 2] = color_b;
                }
            }
        }
    }

    void generateCentredCircleTexture(unsigned char* data,
        const unsigned int width,
        const unsigned int height) {
        for (unsigned int x = 0; x < width; ++x) {
            for (unsigned int y = 0; y < height; ++y) {
                data[3 * (x + width * y) + 0] = 200;
                data[3 * (x + width * y) + 1] = 191;
                data[3 * (x + width * y) + 2] = 231;
            }
        }
        generateCircle(data, width, height, width * 0.5, height * 0.5, width * 0.4, 25, 255, 25);
    }

    void generateQuadsTexture(unsigned char* data,
        const unsigned int width,
        const unsigned int height) {
        for (unsigned int x = 0; x < width; ++x) {
            for (unsigned int y = 0; y < height; ++y) {
                if ((x < width / 2 && y < height / 2) || x >= width / 2 && y >= height / 2) {
                    data[3 * (x + width * y) + 0] = 0;
                    data[3 * (x + width * y) + 1] = 0;
                    data[3 * (x + width * y) + 2] = 0;
                } else {
                    data[3 * (x + width * y) + 0] = 255;
                    data[3 * (x + width * y) + 1] = 255;
                    data[3 * (x + width * y) + 2] = 255;
                }
            }
        }
    }

    const char* vertexShader =
        R"(#version 460
           layout(location = 0) in vec3 vertex_position;
           layout(location = 1) in vec2 texture_coord;

           uniform mat4 model_matrix;
           uniform mat4 view_proj_matrix;
           uniform int anim_frame;
           out vec2 tex_coord_smile;
           out vec2 tex_coord_quads;

           void main() {
              tex_coord_smile = texture_coord;
              tex_coord_quads = texture_coord + vec2(anim_frame / 500.f, anim_frame / 500.f);
              gl_Position = view_proj_matrix * model_matrix * vec4(vertex_position, 1.0);
           })";

    const char* fragmentShader =
        R"(#version 460
           in vec2 tex_coord_smile;
           in vec2 tex_coord_quads;

           layout(binding = 0) uniform sampler2D InTexture_Smile;
           layout(binding = 1) uniform sampler2D InTexture_Quads;
           out vec4 frag_color;

           void main() {
              frag_color = texture(InTexture_Smile, tex_coord_smile) * texture(InTexture_Quads, tex_coord_quads);
           })";

    std::unique_ptr<ShaderProgram> shaderProgram;
    std::unique_ptr<VertexBuffer> cubeVBO;
    std::unique_ptr<IndexBuffer> cubeIndexBuffer;
    std::unique_ptr<Texture2D> circleTexture;
    std::unique_ptr<Texture2D> quadsTexture;
    std::unique_ptr<VertexArray> vao;

	/// <summary>
	/// Occures at app startup (instantiation).
	/// </summary>
	App::App() {
        LOG_INFO("Initializing XEngine App.");
    }

	/// <summary>
	/// Occures on app shutdown (destruction).
	/// </summary>
	App::~App() {
		LOG_INFO("Shuting down XEngine App.");
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
        eventDispatcher.addEventListener<EventWindowResize>([&](EventWindowResize& event) {});
		eventDispatcher.addEventListener<EventWindowClose>([&](EventWindowClose& event) {
			closeWindow = true;
		});
        eventDispatcher.addEventListener<EventMouseMove>([&](EventMouseMove& event) {});
        eventDispatcher.addEventListener<EventKeyDown>([&](EventKeyDown& event) {
            Input::pressKey(event.keyCode);
        });
        eventDispatcher.addEventListener<EventKeyUp>([&](EventKeyUp& event) {
            Input::releaseKey(event.keyCode);
        });
        eventDispatcher.addEventListener<EventMouseButtonDown>([&](EventMouseButtonDown& event) {
            Input::pressMouseKey(event.mouseButton);
        });
        eventDispatcher.addEventListener<EventMouseButtonUp>([&](EventMouseButtonUp& event) {
            Input::releaseMouseKey(event.mouseButton);
        });
		//Apply them.
		mainWindow->setEventCallback([&](BaseEvent& e) {
			eventDispatcher.dispatch(e);
		});

        //Instance all data to draw triangle.
        // TODO: Move to other class.
        //Create texture.
        const unsigned int width = 1000;
        const unsigned int height = 1000;
        const unsigned int channels = 3;
        auto* uvData = new unsigned char[width * height * channels];

        generateCentredCircleTexture(uvData, width, height);
        const GLsizei mipLevels = static_cast<GLsizei>(log2(std::max(width, height))) + 1;
        circleTexture = std::make_unique<Texture2D>(uvData, width, height);
        circleTexture->bind(0);
        generateQuadsTexture(uvData, width, height);
        quadsTexture = std::make_unique<Texture2D>(uvData, width, height);
        quadsTexture->bind(1);
        delete[] uvData;
        //Create shader program.
        shaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
        if (!shaderProgram->isCompilied()) {
            LOG_CRIT("Error while compiling main shader.");
            return -110;
        }
        //Vertex buffers and array.
        BufferLayout bufLayout{
            ShaderDataType::Float3,
            ShaderDataType::Float2
        };
        vao = std::make_unique<VertexArray>();
        cubeVBO = std::make_unique<VertexBuffer>(cubeDataArray, sizeof(cubeDataArray), bufLayout);
        cubeIndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
        //Add buffers to array.
        vao->addVertexBuffer(*cubeVBO);
        vao->setIndexBuffer(*cubeIndexBuffer);
        //Enable depth.
        Renderer::enableDepthTesting();
        static int frame = 0;

        //Update cycle.
		while (!closeWindow) {

            //Clear color buffer.
            Renderer::setClearColorRGB(bgColor[0], bgColor[1], bgColor[2]);
            Renderer::clear();
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
            glm::mat4 positionMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
                position[0], position[1], position[2], 1);
            //Model matrix.
            glm::mat4 model_matrix = scaleMatrix * (rotationXMatrix * rotationYMatrix * rotationZMatrix) * positionMatrix;
            shaderProgram->setMatrix4("model_matrix", model_matrix);
            //shaderProgram->setInt("anim_frame", frame ++);
            //Camera.
            baseCamera.setProjectionMode(camIsPresp ? Camera::ProjectionMode::Perspective :
                Camera::ProjectionMode::Ortographic);
            shaderProgram->setMatrix4("view_proj_matrix",
                baseCamera.getProjectionMatrix() * baseCamera.getViewMatrix());
            //Render.
            Renderer::draw(*vao);
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

    glm::vec2 App::getCursorPosition() const {
        return mainWindow->getMousePos();
    }

}