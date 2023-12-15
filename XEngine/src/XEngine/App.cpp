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

    unsigned int SCR_W = 800, SCR_H = 600;
    float x, y, z;
    float fov = 45.f;

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
        //Set window params.
        SCR_W = t_win_width;
        SCR_H = t_win_height;
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
        GLFWwindow* window = glfwCreateWindow(SCR_W, SCR_H, t_title, NULL, NULL);
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
        glViewport(0, 0, SCR_W, SCR_H);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        //Callbacks for
        glfwSetKeyCallback(window, Keyboard::key_callback);
        glfwSetCursorPosCallback(window, Mouse::cursor_callback);
        glfwSetMouseButtonCallback(window, Mouse::button_callback);
        glfwSetScrollCallback(window, Mouse::scroll_callback);
        //Enable depth test.
        glEnable(GL_DEPTH_TEST);

        //Shader.
        Shader shader("..\\..\\res\\vert_core.glsl", "..\\..\\res\\frag_core.glsl");
        //Vertex array.
        float vertices[] = {
            //    POSITIONS                UVs
            -0.5f, -0.5f, -0.5f,       0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,       1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,       1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,       1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,       0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,       0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,       0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,       1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,       1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,       1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,       0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,       0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,       1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,       1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,       0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,       0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,       0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,       1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,       1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,       1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,       0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,       0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,       0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,       1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,       0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,       1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,       1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,       1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,       0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,       0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,       0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,       1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,       1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,       1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,       0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,       0.0f, 1.0f
        };
        //VAO & VBO & EBO.
        unsigned int vao, vbo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        //Bind arrays.
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        //Set attrib pointers.
        //Positions.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //Texture.
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //Texture.
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);   
            //Draw shape.
            glBindVertexArray(vao);
            //Create transformations.
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.f), glm::vec3(0.5f));
            view = glm::translate(view, glm::vec3(-x, -y, -z));
            projection = glm::perspective(glm::radians(fov), (float)SCR_W / (float)SCR_H, 0.1f, 100.f);
            glm::mat4 transform = projection * view * model;
            //Set shader variables.
            shader.enable();
            shader.set_mat4("transform", transform);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            //Send update to recievers.
            process_input(window);
            update();
        }

        //Terminate libs and rendering//
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glfwTerminate();
    }

    void framebuffer_size_callback(GLFWwindow* t_window, int t_width, int t_height) {
        //Set viewport.
        glViewport(0, 0, t_width, t_height);
        //Update window params.
        SCR_W = t_width;
        SCR_H = t_height;
	}

    void process_input(GLFWwindow* t_window) {
        //On 'Esc' close app.
        if(Keyboard::key_state(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(t_window, true);
        //Default mode.
        if(Keyboard::key_down(GLFW_KEY_1) || main_j.button_state(GLFW_JOYSTICK_DPAD_UP))
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //Wireframe mode.
        if(Keyboard::key_down(GLFW_KEY_2) || main_j.button_state(GLFW_JOYSTICK_DPAD_RIGHT))
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //Keyboard movement.
        float j_x = main_j.axis_state(GLFW_JOYSTICK_AXES_LEFT_STICK_X);
        float j_y = main_j.axis_state(GLFW_JOYSTICK_AXES_LEFT_STICK_Y);
        if (Keyboard::key_state(GLFW_KEY_W) || j_y <= -0.5f)
            z -= 0.1f;
        if (Keyboard::key_state(GLFW_KEY_S) || j_y >= 0.5f)
            z += 0.1f;
        if(Keyboard::key_state(GLFW_KEY_A) || j_x <= -0.5f)
            x -= 0.1f;
        if(Keyboard::key_state(GLFW_KEY_D) || j_x >= 0.5f)
            x += 0.1f;
        if(Keyboard::key_state(GLFW_KEY_UP)) fov += 0.5f;
        if(Keyboard::key_state(GLFW_KEY_DOWN)) fov -= 0.5f;
        if(Mouse::button_down(1)) {
            LOG_INFO("Mouse: btn 1.");
        }
        //Update joystick.
        main_j.update();
    }

}