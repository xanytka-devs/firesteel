#include <iostream>
#include <glad/glad.h>
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
#include "XEngine/Input.hpp"

namespace XEngine {
	
    using namespace XEngine::OpenGL;
    using namespace XEngine::UI;

    GLfloat positions_colors[] = {
        0.0f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,     15.f, -14.f,
        0.0f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,     -14.f, -14.f,
        0.0f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,     15.f,  15.f,
        0.0f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,     -14.f,  15.f
    };

    GLuint indices[] = {
        0, 1, 2, 3, 2, 1
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
           layout(location = 1) in vec3 vertex_color;
           layout(location = 2) in vec2 texture_coord;

           uniform mat4 model_matrix;
           uniform mat4 view_proj_matrix;
           uniform int anim_frame;
           out vec3 color;
           out vec2 tex_coord_smile;
           out vec2 tex_coord_quads;

           void main() {
              color = vertex_color;
              tex_coord_smile = texture_coord;
              tex_coord_quads = texture_coord + vec2(anim_frame / 250.f, anim_frame / 250.f);
              gl_Position = view_proj_matrix * model_matrix * vec4(vertex_position, 1.0);
           })";

    const char* fragmentShader =
        R"(#version 460
           in vec3 color;
           in vec2 tex_coord_smile;
           in vec2 tex_coord_quads;

           layout(binding = 0) uniform sampler2D InTexture_Smile;
           layout(binding = 1) uniform sampler2D InTexture_Quads;
           out vec4 frag_color;

           void main() {
              //frag_color = vec4(color, 1.0);
              frag_color = texture(InTexture_Smile, tex_coord_smile) * texture(InTexture_Quads, tex_coord_quads);
           })";

    std::unique_ptr<ShaderProgram> shaderProgram;
    std::unique_ptr<VertexBuffer> positions_colorsVBO;
    std::unique_ptr<IndexBuffer> indexBuffer;
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
        const GLsizei mipLevels = static_cast<GLsizei>(log2(std::max(width, height))) + 1;
        GLuint textureHandleSmile;
        glCreateTextures(GL_TEXTURE_2D, 1, &textureHandleSmile);
        glTextureStorage2D(textureHandleSmile, mipLevels, GL_RGB8, width, height);
        generateCentredCircleTexture(uvData, width, height);
        glTextureSubImage2D(textureHandleSmile, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, uvData);
        glTextureParameteri(textureHandleSmile, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureHandleSmile, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(textureHandleSmile, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(textureHandleSmile, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateTextureMipmap(textureHandleSmile);
        glBindTextureUnit(0, textureHandleSmile);

        GLuint textureHandleQuads;
        glCreateTextures(GL_TEXTURE_2D, 1, &textureHandleQuads);
        glTextureStorage2D(textureHandleQuads, mipLevels, GL_RGB8, width, height);
        generateQuadsTexture(uvData, width, height);
        glTextureSubImage2D(textureHandleQuads, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, uvData);
        glTextureParameteri(textureHandleQuads, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureHandleQuads, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(textureHandleQuads, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(textureHandleQuads, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateTextureMipmap(textureHandleQuads);
        glBindTextureUnit(1, textureHandleQuads);
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
            ShaderDataType::Float3,
            ShaderDataType::Float2
        };
        vao = std::make_unique<VertexArray>();
        positions_colorsVBO = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), bufLayout);
        indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
        //Add buffers to array.
        vao->addVertexBuffer(*positions_colorsVBO);
        vao->setIndexBuffer(*indexBuffer);

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
            shaderProgram->setInt("anim_frame", frame ++);
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
        glDeleteTextures(1, &textureHandleSmile);
        glDeleteTextures(1, &textureHandleQuads);
		mainWindow = nullptr;

        return 0;

	}

    glm::vec2 App::getCursorPosition() const {
        return mainWindow->getMousePos();
    }

}