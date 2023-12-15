#include <iostream>
#include <string>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "XEngine/App.hpp"
#include "XEngine/Log.hpp"
#include "XEngine/ResManager.hpp"
#include "XEngine/Rendering/Shader.hpp"

#include "XEngine/Input/Keyboard.hpp"
#include "XEngine/Input/Mouse.hpp"
#include "XEngine/Input/Joystick.hpp"

namespace XEngine {

    Joystick main_j(0);

    void framebuffer_size_callback(GLFWwindow* t_window, int t_width, int t_height);
    void process_input(GLFWwindow* t_window);

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
    /// <param name="t_win_width">Width of the window.</param>
    /// <param name="t_win_height">Height of the window.</param>
    /// <param name="t_title">Name of the window.</param>
    /// <returns>Exit code. Only 0 is success.</returns>
    int App::start(unsigned int t_win_width, unsigned int t_win_height, const char* t_title) {

        //Initiate core//
        glfwInit();
        //Set version to 3.3.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //Set profile.
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        //Create window.
        GLFWwindow* window = glfwCreateWindow(t_win_width, t_win_height, t_title, NULL, NULL);
        if (window == NULL) {
            LOG_CRIT("Failed to create GLFW window.");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        //Load GLAD.
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRIT("Failed to initialize GLAD.");
            return -1;
        }
        //Set viewport.
        glViewport(0, 0, 800, 600);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        //Callbacks for
        glfwSetKeyCallback(window, Keyboard::key_callback);
        glfwSetCursorPosCallback(window, Mouse::cursor_callback);
        glfwSetMouseButtonCallback(window, Mouse::button_callback);
        glfwSetScrollCallback(window, Mouse::scroll_callback);

        //Shader.
        Shader shader("..\\..\\res\\vert_core.glsl", "..\\..\\res\\frag_core.glsl");
        //Vertex array.
        float vertices[] = {
            //    POSITIONS              COLORS             UVs
            -0.5f, -0.5f,  0.0f,    0.f, 0.f, 0.f,      0.0f, 0.0f,
            -0.5f,  0.5f,  0.0f,    0.f, 0.f, 0.f,      0.0f, 1.0f,
             0.5f, -0.5f,  0.0f,    0.f, 0.f, 0.f,      1.0f, 0.0f,
             0.5f,  0.5f,  0.0f,    0.f, 0.f, 0.f,      1.0f, 1.0f,
        };
        unsigned int indices[] = {
            0, 1, 2, //First t.
            3, 1, 2  //Second t.
        };
        //VAO & VBO & EBO.
        unsigned int vao, vbo, ebo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        //Bind arrays.
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        //Setup EBO.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        //Set attrib pointers.
        //Positions.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Colors.
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //Texture.
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        //Load textures.
        unsigned int texture1;
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        //Texture wrap.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //Texture mipmap.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //Set texture.
        int width = 1000;
        int height = 1000;
        int channels = 3;
        unsigned char* texture = ResManager::load_image("..\\..\\res\\quads_diffusion.png", &width, &height, &channels);
        if (texture) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        ResManager::free_image(texture);

        //Joystick checks.
        main_j.update();
        if (main_j.is_present()) {
            //If present - send joystick's name.
            std::stringstream msg;
            msg << "Controller: '" << main_j.get_name() << "'.";
            LOG_INFO(msg.str().c_str());
        } else {
            //If not - just send 'None'.
            LOG_INFO("Controller: None.");
        }

        //Update loop//
        while(!glfwWindowShouldClose(window)) {
            //Window processes.
            glfwSwapBuffers(window);
            glfwPollEvents();
            //Set clear color.
            glClearColor(0.15f, 0.15f, 0.15f, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            //Rotate matrix.
            /*trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0, 0, 1));
            shader.set_mat4("transform", trans);*/
            shader.enable();
            shader.set_int("albedo", 0);
            //Draw shape.
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            //Send update to recievers.
            process_input(window);
            update();
        }

        //Terminate libs and rendering//
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glfwTerminate();
    }

    void framebuffer_size_callback(GLFWwindow* t_window, int t_width, int t_height) {
        glViewport(0, 0, t_width, t_height); //Set viewport.
	}

    void process_input(GLFWwindow* t_window) {
        //On 'Esc' close app.
        if(Keyboard::key_status(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(t_window, true);
        //Default mode.
        if(Keyboard::key_down(GLFW_KEY_1) || main_j.button_state(GLFW_JOYSTICK_DPAD_UP))
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //Wireframe mode.
        if(Keyboard::key_down(GLFW_KEY_2) || main_j.button_state(GLFW_JOYSTICK_DPAD_RIGHT))
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //Update joystick.
        main_j.update();
    }

}