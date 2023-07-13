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
        //   Position              Normal               UV        Index
        //    FRONT
        -1.0f, -1.f, -1.f,    -1.f,  0.f,  0.f,      0.f, 0.f,    /*0*/
        -1.0f,  1.f, -1.f,    -1.f,  0.f,  0.f,      1.f, 0.f,    /*1*/
        -1.0f,  1.f,  1.f,    -1.f,  0.f,  0.f,      1.f, 1.f,    /*2*/
        -1.0f, -1.f,  1.f,    -1.f,  0.f,  0.f,      0.f, 1.f,    /*3*/
        //     BACK                                  
         1.0f, -1.f, -1.f,     1.f,  0.f,  0.f,      1.f, 0.f,    /*4*/
         1.0f,  1.f, -1.f,     1.f,  0.f,  0.f,      0.f, 0.f,    /*5*/
         1.0f,  1.f,  1.f,     1.f,  0.f,  0.f,      0.f, 1.f,    /*6*/
         1.0f, -1.f,  1.f,     1.f,  0.f,  0.f,      1.f, 1.f,    /*7*/
         //    RIGHT
         -1.0f,  1.f, -1.f,     0.f,  1.f,  0.f,     0.f, 0.f,    /*8*/
          1.0f,  1.f, -1.f,     0.f,  1.f,  0.f,     1.f, 0.f,    /*9*/
          1.0f,  1.f,  1.f,     0.f,  1.f,  0.f,     1.f, 1.f,    /*10*/
         -1.0f,  1.f,  1.f,     0.f,  1.f,  0.f,     0.f, 1.f,    /*11*/
         //    LEFT
         -1.0f, -1.f, -1.f,     0.f, -1.f,  0.f,     1.f, 0.f,    /*12*/
          1.0f, -1.f, -1.f,     0.f, -1.f,  0.f,     0.f, 0.f,    /*13*/
          1.0f, -1.f,  1.f,     0.f, -1.f,  0.f,     0.f, 1.f,    /*14*/
         -1.0f, -1.f,  1.f,     0.f, -1.f,  0.f,     1.f, 1.f,    /*15*/
         //    TOP
         -1.0f, -1.f,  1.f,     0.f,  0.f,  1.f,     0.f, 0.f,    /*16*/
         -1.0f,  1.f,  1.f,     0.f,  0.f,  1.f,     1.f, 0.f,    /*17*/
          1.0f,  1.f,  1.f,     0.f,  0.f,  1.f,     1.f, 1.f,    /*18*/
          1.0f, -1.f,  1.f,     0.f,  0.f,  1.f,     0.f, 1.f,    /*19*/
          //  BOTTOM
          -1.0f, -1.f, -1.f,    0.f,  0.f, -1.f,     0.f, 1.f,    /*20*/
          -1.0f,  1.f, -1.f,    0.f,  0.f, -1.f,     1.f, 1.f,    /*21*/
           1.0f,  1.f, -1.f,    0.f,  0.f, -1.f,     1.f, 0.f,    /*22*/
           1.0f, -1.f, -1.f,    0.f,  0.f, -1.f,     0.f, 0.f,    /*23*/
    };

    GLuint indices[] = {
        0, 1, 2, 2, 3, 0,       //FRONT
        4, 5, 6, 6,  7, 4,      //BACK
        8, 9, 10, 10, 11, 8,    //RIGHT
        12, 13, 14, 14, 15, 12, //LEFT
        16, 17, 18, 18, 19, 16, //TOP
        20, 21, 22, 22, 23, 20  //BOTTOM
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
           layout(location = 1) in vec3 vertex_normal;
           layout(location = 2) in vec2 texture_coord;

           uniform mat4 model_matrix;
           uniform mat4 view_proj_matrix;
           uniform int anim_frame;

           out vec2 tex_coord_circle;
           out vec2 tex_coord_quads;
           out vec3 frag_position;
           out vec3 frag_normal;

           void main() {
              tex_coord_circle = texture_coord;
              tex_coord_quads = texture_coord + vec2(anim_frame / 500.f, anim_frame / 500.f);
              frag_normal = mat3(transpose(inverse(model_matrix))) * vertex_normal;
              vec4 vertex_pos_world = model_matrix * vec4(vertex_position, 1.0);
              frag_position = vertex_pos_world.xyz;
              gl_Position = view_proj_matrix * vertex_pos_world;
           })";

    const char* fragmentShader =
        R"(#version 460
           in vec2 tex_coord_circle;
           in vec2 tex_coord_quads;
           in vec3 frag_position;
           in vec3 frag_normal;

           layout(binding = 0) uniform sampler2D InTexture_Circle;
           layout(binding = 1) uniform sampler2D InTexture_Quads;

           uniform vec3 camera_position;
           uniform vec3 light_color;
           uniform vec3 light_position;
           uniform float ambient_factor;
           uniform float diffuse_factor;
           uniform float specular_factor;
           uniform float shininess;

           out vec4 frag_color;

           void main() {
              //Ambient
              vec3 ambient = ambient_factor * light_color;
              //Diffuse
              vec3 normal = normalize(frag_normal);
              vec3 light_dir = normalize(light_position - frag_position);
              vec3 diffuse = diffuse_factor * light_color * max(dot(normal, light_dir), 0.0);
              //Specular
              vec3 camera_dir = normalize(camera_position - frag_position);
              vec3 reflect_dir = reflect(-light_dir, normal);
              float specular_value = pow(max(dot(camera_dir, reflect_dir), 0.0), shininess);
              vec3 specular = specular_factor * specular_value * light_color;
              //Final
              frag_color = texture(InTexture_Circle, tex_coord_circle)
                * texture(InTexture_Quads, tex_coord_quads)
                * vec4(ambient + diffuse + specular, 1.f);
           })";

    const char* lsVertexShader =
        R"(#version 460
           layout(location = 0) in vec3 vertex_position;
           layout(location = 1) in vec3 vertex_normal;
           layout(location = 2) in vec2 texture_coord;

           uniform mat4 model_matrix;
           uniform mat4 view_projection_matrix;

           void main() {
              gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position * 0.1f, 1.0);
           }
        )";

    const char* lsFragmentShader =
        R"(#version 460
           out vec4 frag_color;
           uniform vec3 light_color;

           void main() {
              frag_color = vec4(light_color, 1.f);
           }
        )";

    std::unique_ptr<ShaderProgram> shaderProgram;
    std::unique_ptr<ShaderProgram> lsShaderProgram;
    std::unique_ptr<VertexBuffer> cubeVBO;
    std::unique_ptr<IndexBuffer> cubeIndexBuffer;
    std::unique_ptr<Texture2D> circleTexture;
    std::unique_ptr<Texture2D> quadsTexture;
    std::unique_ptr<VertexArray> vao;
    std::array<glm::vec3, 5> positions = {
            glm::vec3(-2.f, -2.f, -4.f),
            glm::vec3(-5.f,  0.f,  3.f),
            glm::vec3(2.f,  1.f, -2.f),
            glm::vec3(4.f, -3.f,  3.f),
            glm::vec3(1.f, -7.f,  1.f)
    };

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
        baseCamera.setViewportSize(static_cast<float>(win_width),static_cast<float>(win_height));
		//Add event listeners.
        eventDispatcher.addEventListener<EventWindowResize>([&](EventWindowResize& event) {
            baseCamera.setViewportSize(event.width, event.height);
            App::draw();
        });
        eventDispatcher.addEventListener<EventWindowClose>([&](EventWindowClose& event) {
            App::exit();
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
        lsShaderProgram = std::make_unique<ShaderProgram>(lsVertexShader, lsFragmentShader);
        if (!lsShaderProgram->isCompilied()) {
            LOG_CRIT("Error while compiling light shader.");
            return -110;
        }
        //Vertex buffers and array.
        BufferLayout bufLayout{
            ShaderDataType::Float3,
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
        onInitialized();
        //Update cycle.
		while (!closeWindow) {
            draw();
			mainWindow->update();
			update();
		}
		mainWindow = nullptr;

        return 0;

	}

    void App::draw() {
        static int frame = 0;
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
        if(!disableAnimations)
            shaderProgram->setInt("anim_frame", frame++);
        shaderProgram->setMatrix4("view_proj_matrix",
            baseCamera.getProjectionMatrix() * baseCamera.getViewMatrix());
        shaderProgram->setVector3("light_color", glm::vec3(lightSourceColor[0], lightSourceColor[1], lightSourceColor[2]));
        shaderProgram->setVector3("light_position", glm::vec3(lightSourcePos[0], lightSourcePos[1], lightSourcePos[2]));
        shaderProgram->setVector3("camera_position", baseCamera.getPosition());
        shaderProgram->setFloat("ambient_factor", ambientFactor);
        shaderProgram->setFloat("diffuse_factor", diffuseFactor);
        shaderProgram->setFloat("specular_factor", specularFactor);
        shaderProgram->setFloat("shininess", shininess);
        //Render.
        Renderer::draw(*vao);
        for (const glm::vec3& curPos : positions) {
            glm::mat4 translateMatrix(1, 0, 0, 0, 0, 1, 0, 0,
                0, 0, 1, 0, curPos[0], curPos[1], curPos[2], 1);
            shaderProgram->setMatrix4("model_matrix", translateMatrix);
            Renderer::draw(*vao);
        }
        //Light source.        
        lsShaderProgram->bind();
        lsShaderProgram->setMatrix4("view_projection_matrix", baseCamera.getProjectionMatrix() * baseCamera.getViewMatrix());
        glm::mat4 lsTranslateMatrix(1, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 1, 0, lightSourcePos[0], lightSourcePos[1], lightSourcePos[2], 1);
        lsShaderProgram->setMatrix4("model_matrix", lsTranslateMatrix);
        lsShaderProgram->setVector3("light_color", glm::vec3(lightSourceColor[0], lightSourceColor[1], lightSourceColor[2]));
        Renderer::draw(*vao);
        //Create new frame for ImGui.
        TUI::update();
        bool eADSshow = true;
        //Draw ImGui.
        onUIDraw();
        //Render ImGui.
        TUI::draw();
    }

    glm::vec2 App::getCursorPosition() const {
        return mainWindow->getMousePos();
    }

    void App::exit() {
        closeWindow = true;
    }

}