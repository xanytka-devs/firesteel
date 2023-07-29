#include <iostream>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/trigonometric.hpp>

#include "XEngine/App.hpp"
#include "XEngine/Event.hpp"
#include "XEngine/Window.hpp"
#include "XEngine/Log.hpp"
#include "XEngine/UI/TUI.hpp"
#include "XEngine/ResLoader.hpp"
#include "XEngine/Rendering/OpenGL/ShaderProgram.hpp"
#include "XEngine/Rendering/OpenGL/VertexBuffer.hpp"
#include "XEngine/Rendering/OpenGL/IndexBuffer.hpp"
#include "XEngine/Rendering/OpenGL/VertexArray.hpp"
#include "XEngine/Rendering/OpenGL/Renderer.hpp"
#include "XEngine/Rendering/OpenGL/Texture2D.hpp"
#include "XEngine/Camera.hpp"
#include "XEngine/Input/Input.hpp"
#include "Rendering/OpenGL/Material.hpp"
#include "XEngine/SceneManager.hpp"

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

    std::unique_ptr<VertexBuffer> cubeVBO;
    std::unique_ptr<IndexBuffer> cubeIndexBuffer;
    std::unique_ptr<Material> cubeMaterial;
    std::unique_ptr<Material> lightSourceMaterial;
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<SceneManager> manager;
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
		LOG_INFO("Shutting down XEngine App.");
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
            baseCamera.setViewportSize(static_cast<float>(event.width), static_cast<float>(event.height));
            App::draw();
        });
        eventDispatcher.addEventListener<EventWindowClose>([&](EventWindowClose& event) {
            App::exit();
		});
        eventDispatcher.addEventListener<EventMouseMove>([&](EventMouseMove& event) {
            Input::setMousePosition(glm::vec2(event.x, event.y));
        });
        eventDispatcher.addEventListener<EventMouseScroll>([&](EventMouseScroll& event) {
            Input::setMouseScroll(glm::vec2(event.x, event.y));
        });
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
        //Instantiate scene.
        //Scene main = SceneManager::Instance->addScene();
        //manager = std::make_unique<SceneManager>();
        //Create texture.
        int width = 100;
        int height = 100;
        int channels = 3;
        unsigned char* uvData = new unsigned char[width * height * channels];
        ResLoader::flipImagesVertical(true);
        std::string vScode = ResLoader::loadText("../../res/geometry.vert");
        std::string fScode = ResLoader::loadText("../../res/geometry.frag");
        const char* vertexShader = vScode.c_str();
        const char* fragmentShader = fScode.c_str();
        cubeMaterial = std::make_unique<Material>("Geometry", vScode.c_str(), fScode.c_str());
        vScode = ResLoader::loadText("../../res/light_source.vert");
        fScode = ResLoader::loadText("../../res/light_source.frag");
        vertexShader = vScode.c_str();
        fragmentShader = fScode.c_str();
        lightSourceMaterial = std::make_unique<Material>("Light", vScode.c_str(), fScode.c_str());
        //Load quads diffusion map.
        uvData = ResLoader::loadImage("../../res/quads_diffusion.png", &width, &height, &channels, 3);
        cubeMaterial->addTexture(new Texture2D(uvData, width, height));
        //Load quads specular map.
        uvData = ResLoader::loadImage("../../res/quads_specular.png", &width, &height, &channels, 3);
        cubeMaterial->addTexture(new Texture2D(uvData, width, height));
        //Load quads emission map.
        uvData = ResLoader::loadImage("../../res/quads_emission.png", &width, &height, &channels, 3);
        cubeMaterial->addTexture(new Texture2D(uvData, width, height));
        delete[] uvData;
        //Create shader program.
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
        baseCamera.setRotation(glm::vec3(0.f, 3.f, -60.f));
        onInitialized();
        //Update cycle.
		while (!closeWindow) {
            draw();
		}
		mainWindow = nullptr;
        return 0;
	}

    void App::draw() {
        //Update scenes.
        //manager->update();
        //Clear color buffer.
        Renderer::setClearColorRGB(bgColor[0], bgColor[1], bgColor[2]);
        Renderer::clear();
        // TODO: Move to other class.
        //Render triangle.
        cubeMaterial->bind();
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
        cubeMaterial->setVector3("light_position", glm::vec3(baseCamera.getViewMatrix()
            * glm::vec4(lightSourcePos[0], lightSourcePos[1], lightSourcePos[2], 1.f)));
        cubeMaterial->setVector3("light_color", glm::vec3(lightSourceColor[0], lightSourceColor[1], lightSourceColor[2]));
        cubeMaterial->setFloat("ambient_factor", ambientFactor);
        cubeMaterial->setFloat("diffuse_factor", diffuseFactor);
        cubeMaterial->setFloat("specular_factor", specularFactor);
        cubeMaterial->setFloat("shininess", shininess);
        cubeMaterial->setFloat("emission_factor", emission);
        cubeMaterial->setVector3("emission_color", glm::vec3(emissionColor[0], emissionColor[1], emissionColor[2]));
        //Render.
        Renderer::draw(*vao);
        for (const glm::vec3& curPos : positions) {
            glm::mat4 translateMatrix(1, 0, 0, 0, 0, 1, 0, 0,
                0, 0, 1, 0, curPos[0], curPos[1], curPos[2], 1);
            const glm::mat4 modelViewMatrix = baseCamera.getViewMatrix() * translateMatrix;
            cubeMaterial->setMatrix4("model_view_matrix", modelViewMatrix);
            cubeMaterial->setMatrix4("mvp_matrix", baseCamera.getProjectionMatrix() * modelViewMatrix);
            cubeMaterial->setMatrix3("normal_matrix", glm::transpose(glm::inverse(glm::mat3(modelViewMatrix))));
            Renderer::draw(*vao);
        }
        //Light source.        
        lightSourceMaterial->bind();
        glm::mat4 lsTranslateMatrix(1, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 1, 0, lightSourcePos[0], lightSourcePos[1], lightSourcePos[2], 1);
        lightSourceMaterial->setMatrix4("mvp_matrix", baseCamera.getProjectionMatrix() * baseCamera.getViewMatrix() * lsTranslateMatrix);
        lightSourceMaterial->setVector3("light_color", glm::vec3(lightSourceColor[0], lightSourceColor[1], lightSourceColor[2]));
        Renderer::draw(*vao);
        //Create new frame for ImGui.
        TUI::update();
        bool eADSshow = true;
        //Draw ImGui.
        onUIDraw();
        //Render ImGui.
        TUI::draw();
        mainWindow->update();
        update();
    }

    glm::vec2 App::getCursorPosition() const {
        return mainWindow->getMousePos();
    }

    void App::exit() {
        App::onExit();
        closeWindow = true;
    }

}