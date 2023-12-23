#include <memory>
#include <sstream>
#include <iostream>

#include <XEngine/App.hpp>
#include <XEngine/Input/Keyboard.hpp>
#include <XEngine/Input/Mouse.hpp>
#include <XEngine/Input/Joystick.hpp>
#include <XEngine/Rendering/Renderer.hpp>
#include <XEngine/Rendering/Camera.hpp>
#include "XEngine/Rendering/Shader.hpp"
#include "XEngine/Rendering/Window.hpp"
#include "XEngine/Rendering/Texture.hpp"
#include "XEngine/Components/Transform.hpp"
#include "XEngine/Components/LightSource.hpp"

Joystick main_j(0);
XEngine::Shader box_shader;
XEngine::Shader light_shader;
XEngine::Transform box(glm::vec3(0, 0, 0), glm::vec4(0, 0, 0, 1), glm::vec3(0.75f), Material::bronze);
XEngine::LightSource light(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f),
    glm::vec3(-1.0f, -.5f, .5f), glm::vec4(0, 0, 0, 1), glm::vec3(0.25f));
XEngine::Camera camera(glm::vec3(0.f, 0.f, 3.f));

class EditorApp : public XEngine::App {

    virtual void initiazile() override {
        //Joystick checks.
        main_j.update();
        //Cursor state.
        window.set_cursor_state(XEngine::CursorState::DISABLED);
        //Model.
        box_shader = XEngine::Shader("..\\..\\res\\object_vert.glsl", "..\\..\\res\\object_frag.glsl");
        box.initialize();
        //Light source.
        light_shader = XEngine::Shader("..\\..\\res\\object_vert.glsl", "..\\..\\res\\light_frag.glsl");
        light.initialize();
    }

    int mode = 0;
	virtual void update() override {
        //Create transformations.
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)window.width / (float)window.height, 0.1f, 100.f);
        //Render model.
        box_shader.enable();
        box_shader.set_3_floats("view_pos", camera.position);
        box_shader.set_3_floats("light.position", light.position);
        box_shader.set_3_floats("light.ambient", light.ambient);
        box_shader.set_3_floats("light.diffuse", light.diffuse);
        box_shader.set_3_floats("light.specular", light.specular);
        box_shader.set_mat4("projection", projection);
        box_shader.set_mat4("view", view);
        box_shader.set_int("render_mode", mode);
        box.render(box_shader);
        //Render light.
        light_shader.enable();
        light_shader.set_mat4("projection", projection );
        light_shader.set_mat4("view", view);
        light_shader.set_int("render_mode", mode);
        light.render(light_shader);
        //Take care of input.
        input();
    }

    void input() {
        //On 'Esc' close app.
        if(Keyboard::key_state(KeyCode::ESCAPE) || main_j.button_state(JoystickControls::J_HOME))
            App::shutdown();
        //Default mode.
        if(Keyboard::key_down(KeyCode::KEY_1) || main_j.button_state(JoystickControls::DPAD_LEFT)) { mode = 0;
            XEngine::Renderer::switch_mode(XEngine::RenderMode::DEFAULT);
        }
        //Wireframe mode.
        if(Keyboard::key_down(KeyCode::KEY_2) || main_j.button_state(JoystickControls::DPAD_RIGHT)) { mode = 0;
            XEngine::Renderer::switch_mode(XEngine::RenderMode::WIREFRAME);
        }
        //UV mode.
        if(Keyboard::key_down(KeyCode::KEY_3)) { mode = 1;
            XEngine::Renderer::switch_mode(XEngine::RenderMode::DEFAULT);
        }
        //Normal mode.
        if(Keyboard::key_down(KeyCode::KEY_4)) { mode = 2;
            XEngine::Renderer::switch_mode(XEngine::RenderMode::DEFAULT);
        }
        //Keyboard movement.
        float j_x = main_j.axis_state(JoystickControls::AXES_LEFT_STICK_X);
        float j_y = main_j.axis_state(JoystickControls::AXES_LEFT_STICK_Y);
        //Position changes.
        // F/B movement.
        if (Keyboard::key_state(KeyCode::W) || j_y <= -0.5f)
            camera.position += camera.forward * (App::delta_time * 2.5f);
        if (Keyboard::key_state(KeyCode::S) || j_y >= 0.5f)
            camera.position -= camera.forward * (App::delta_time * 2.5f);
        // R/L movement.
        if (Keyboard::key_state(KeyCode::D) || j_x >= 0.5f)
            camera.position += camera.right * (App::delta_time * 2.5f);
        if (Keyboard::key_state(KeyCode::A) || j_x <= -0.5f)
            camera.position -= camera.right * (App::delta_time * 2.5f);
        // U/D movement.
        if (Keyboard::key_state(KeyCode::SPACE) || main_j.button_state(JoystickControls::DPAD_UP))
            camera.position += camera.up * (App::delta_time * 2.5f);
        if (Keyboard::key_state(KeyCode::LEFT_SHIFT) || main_j.button_state(JoystickControls::DPAD_DOWN))
            camera.position -= camera.up * (App::delta_time * 2.5f);
        //Camera rotation.
        double dx = Mouse::get_cursor_dx(), dy = Mouse::get_cursor_dy();
        if(dx != 0 || dy != 0) camera.update_direction(dx, dy);
        else if(main_j.is_present()) {
            dx = main_j.axis_state(JoystickControls::AXES_RIGHT_STICK_X);
            dy = -main_j.axis_state(JoystickControls::AXES_RIGHT_STICK_Y);
            if (dx >= 0.5 || dy >= 0.5 || dx <= -0.5 || dy <= -0.5) camera.update_direction(dx, dy);
        }
        //Camera zoom.
        double mouse_zoom = Mouse::get_wheel_dy();
        if(camera.fov >= 1.f && camera.fov <= 45.f) camera.fov -= mouse_zoom;
        else if(camera.fov < 1.f) camera.fov = 1.f;
        else camera.fov = 45.f;
        //Update joystick.
        main_j.update();
    }

    virtual void on_shutdown() override {
        box.remove();
        box_shader.remove();
        light.remove();
        light_shader.remove();
    }

};

int main() {

    auto editor = std::make_unique<EditorApp>();

    int returnC = editor->start(800, 600, "Hello XEngine!");
    
    return 0;

}