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
#include "XEngine/Rendering/Camera.hpp"

#include "XEngine/Input/Keyboard.hpp"
#include "XEngine/Input/Mouse.hpp"
#include "XEngine/Input/Joystick.hpp"

namespace XEngine {

    Joystick main_j(0);

    void framebuffer_size_callback(GLFWwindow* t_window, int t_width, int t_height);
    void process_input(double t_delta_time);
    bool App::m_quit = false;
    Camera camera(glm::vec3(0.f, 0.f, 3.f));
    float delta_time = 0.f;
    float last_frame = 0.f;
    unsigned int SCR_W = 800, SCR_H = 600;

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
    /// 
    /// </summary>
    void App::shutdown() {
        m_quit = true;
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
        m_window = glfwCreateWindow(SCR_W, SCR_H, t_title, NULL, NULL);
        if (m_window == NULL) {
            LOG_CRIT("Failed to create GLFW window.");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(m_window);
        //Load GLAD.
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRIT("Failed to initialize GLAD.");
            return -1;
        }
        //Set viewport.
        glViewport(0, 0, SCR_W, SCR_H);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        //Callbacks for input.
        glfwSetKeyCallback(m_window, Keyboard::key_callback);
        glfwSetCursorPosCallback(m_window, Mouse::cursor_callback);
        glfwSetMouseButtonCallback(m_window, Mouse::button_callback);
        glfwSetScrollCallback(m_window, Mouse::scroll_callback);
        //Disable cursor.
        //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
        while(!glfwWindowShouldClose(m_window) && !m_quit) {
            //Update delta time.
            double cur_time = glfwGetTime();
            delta_time = cur_time - last_frame;
            last_frame = cur_time;
            //Window processes.
            glfwSwapBuffers(m_window);
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
            view = camera.get_view_matrix();
            projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_W / (float)SCR_H, 0.1f, 100.f);
            glm::mat4 transform = projection * view * model;
            //Set shader variables.
            shader.enable();
            shader.set_mat4("transform", transform);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            //Send update to recievers.
            process_input(delta_time);
            update();
        }

        //Terminate libs and rendering//
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glfwTerminate();
        return 0;
    }

    void framebuffer_size_callback(GLFWwindow* t_window, int t_width, int t_height) {
        //Set viewport.
        glViewport(0, 0, t_width, t_height);
        //Update window params.
        SCR_W = t_width;
        SCR_H = t_height;
	}

    void process_input(double t_delta_time) {
        //On 'Esc' close app.
        if(Keyboard::key_state(KeyCode::ESCAPE))
            App::shutdown();
        //Default mode.
        if(Keyboard::key_down(KeyCode::KEY_1) || main_j.button_state(JoystickControls::DPAD_LEFT))
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //Wireframe mode.
        if(Keyboard::key_down(KeyCode::KEY_2) || main_j.button_state(JoystickControls::DPAD_RIGHT))
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //Keyboard movement.
        float j_x = main_j.axis_state(JoystickControls::AXES_LEFT_STICK_X);
        float j_y = main_j.axis_state(JoystickControls::AXES_LEFT_STICK_Y);
        //Position changes.
        if(Keyboard::key_state(KeyCode::W) || j_y <= -0.5f)
            camera.update_position(Direction::FORWARD, t_delta_time);
        if(Keyboard::key_state(KeyCode::S) || j_y >= 0.5f)
            camera.update_position(Direction::BACK, t_delta_time);
        if(Keyboard::key_state(KeyCode::A) || j_x <= -0.5f)
            camera.update_position(Direction::LEFT, t_delta_time);
        if(Keyboard::key_state(KeyCode::D) || j_x >= 0.5f)
            camera.update_position(Direction::RIGHT, t_delta_time);
        if (Keyboard::key_state(KeyCode::SPACE) || main_j.button_state(JoystickControls::DPAD_UP))
            camera.update_position(Direction::UP, t_delta_time);
        if (Keyboard::key_state(KeyCode::LEFT_SHIFT) || main_j.button_state(JoystickControls::DPAD_DOWN))
            camera.update_position(Direction::DOWN, t_delta_time);
        //Camera rotation.
        double dx = Mouse::get_cursor_dx(), dy = Mouse::get_cursor_dy();
        if(dx != 0 || dy != 0) camera.update_direction(dx * 0.45, dy * 0.45);
        else {
            dx = main_j.axis_state(JoystickControls::AXES_RIGHT_STICK_X);
            dy = -main_j.axis_state(JoystickControls::AXES_RIGHT_STICK_Y);
            if (dx != 0 || dy != 0) camera.update_direction(dx, dy);
        }
        //Camera zoom.
        double mouse_zoom = Mouse::get_wheel_dy();
        if(mouse_zoom != 0) camera.update_zoom(mouse_zoom);
        if(Mouse::button_down(1)) {
            LOG_INFO("Mouse: btn 1.");
        }
        //Update joystick.
        main_j.update();
    }

}