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
#include "XEngine/Rendering/Window.hpp"
#include "XEngine/Rendering/Texture.hpp"
#include "XEngine/Components/Transform.hpp"

#include "XEngine/Input/Keyboard.hpp"
#include "XEngine/Input/Mouse.hpp"
#include "XEngine/Input/Joystick.hpp"

namespace XEngine {

    Window window;
    Joystick main_j(0);


    void process_input(double t_delta_time);
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
    /// Quits app.
    /// </summary>
    void App::shutdown() {
        window.close();
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
        if (!window.initialize()) {
            LOG_CRIT("Failed to create GLFW window.");
            glfwTerminate();
            return -1;
        }
        //Load GLAD.
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRIT("Failed to initialize GLAD.");
            return -1;
        }
        //Set parameters.
        window.set_params();
        window.set_cursor_state(CursorState::DISABLED);

        //Model.
        Shader shader("..\\..\\res\\object_vert.glsl", "..\\..\\res\\object_frag.glsl");
        Transform box(glm::vec3(0, 0, 0), glm::vec4(0, 0, 0, 1), glm::vec3(0.75f));
        box.initialize();

        //Joystick checks.
        main_j.update();
        if (main_j.is_present()) {
            //If present - send joystick's name.
            std::stringstream msg;
            msg << "Controller: '" << main_j.get_name() << "'.";
            LOG_INFO(msg.str().c_str());
        } else
            //If not - just send 'None'.
            LOG_INFO("Controller: None.");

        //Update loop//
        while(!window.closing()) {
            //Update delta time.
            double cur_time = glfwGetTime();
            delta_time = cur_time - last_frame;
            last_frame = cur_time;
            //Window processes.
            window.update();
            //Shader.
            shader.enable();
            //Create transformations.
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);
            view = camera.get_view_matrix();
            projection = glm::perspective(glm::radians(camera.fov), (float)SCR_W / (float)SCR_H, 0.1f, 100.f);
            shader.set_mat4("transform", projection * view);
            //Render model.
            box.render(shader);
            //Send update to recievers.
            process_input(delta_time);
            update();
        }

        //Terminate libs and rendering//
        box.remove();
        glfwTerminate();
        return 0;
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
        if (camera.fov >= 1.f && camera.fov <= 45.f) camera.fov -= mouse_zoom;
        else if (camera.fov < 1.f) camera.fov = 1.f;
        else camera.fov = 45.f;
        if(Mouse::button_down(1)) {
            LOG_INFO("Mouse: btn 1.");
        }
        //Update joystick.
        main_j.update();
    }

}