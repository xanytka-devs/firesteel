#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>

#include <xengine/app.hpp>
#include <xengine/utils.hpp>
#include <xengine/audio.hpp>
#include <xengine/input.hpp>
#include <xengine/rendering/renderer.hpp>
#include <xengine/rendering/camera.hpp>
#include <xengine/rendering/shader.hpp>
#include <xengine/rendering/window.hpp>
#include <xengine/rendering/texture.hpp>
#include <xengine/rendering/light.hpp>
#include <xengine/rendering/transform.hpp>

#include "components/light_source.hpp"
#include "components/billboard.hpp"
#include "ui.hpp"

using namespace XEngine;

Joystick main_j(0);
Shader box_shader;
Shader light_shader;
Shader gizmo_shader;
Transform model(glm::vec3(0.f), glm::vec4(glm::vec3(0.f), 1.f), glm::vec3(0.05f));
glm::vec3 point_light_positions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
};
const int point_lights_amount = (int)(sizeof(point_light_positions) / sizeof(glm::vec3));
LightSource lights[point_lights_amount];
Camera camera(glm::vec3(-1.8f, 3.07f, 3.82f), -56.f, -36.f);
DirectionalLight dir_light = { glm::vec3(-0.2f, -1.f, -0.3f),
    glm::vec4(0.0f), glm::vec4(1.4f), glm::vec4(0.75f), glm::vec4(0.7f, 0.5f, 0.35f, 1.0f) };
SpotLight spot_light = { camera.position, camera.forward, glm::cos(glm::radians(12.5f)),
    glm::cos(glm::radians(20.f)), 1.0f, 0.07f, 0.032f, glm::vec4(0.f), glm::vec4(1.f),
    glm::vec4(1.f), glm::vec4(1.f) };
Audio a{ "..\\..\\res\\sound.wav", false, {"test", 100.f, 2.f}};
Billboard dir_light_gizmo(glm::vec3(0.f, 1.f, 0.f), glm::vec4(glm::vec3(0.f), 1.f), glm::vec3(0.5f),
    &camera, "..\\..\\res\\gizmos\\dir_light.png");

class EditorApp : public App {

    virtual void initiazile() override {
#ifndef NDEBUG
        printf("\nXEngine Editor\nDebug Output\n\n");
#endif
        //Print renderer summary.
        window.set_param(W_VSYNC, true);
        Renderer::print_host_info();
        //Initialize audio manager.
        AudioManager::initialize();
        AudioManager::print_host_info();
        over_init();
        //Joystick checks.
        main_j.update();
        //Initialize ImGui.
        window.ui_initialize();
        UI::update_pos(&camera);
    }

    void over_init() {
        //Model.
        model.load_model("..\\..\\res\\sphere\\scene.gltf");
        box_shader = Shader("..\\..\\res\\object_vert.glsl", "..\\..\\res\\object_frag.glsl");
        //Light source.
        light_shader = Shader("..\\..\\res\\object_vert.glsl", "..\\..\\res\\light_frag.glsl");
        for (unsigned int i = 0; i < point_lights_amount; i++) {
            lights[i] = LightSource(glm::vec4(1.0f), glm::vec4(0.0f), glm::vec4(1.0f), glm::vec4(1.0f),
                1.0f, 0.07f, 0.032f, point_light_positions[i], glm::vec4(0, 0, 0, 1), glm::vec3(0.25f));
            lights[i].initialize();
        }
        //Gizmo.
        gizmo_shader = Shader("..\\..\\res\\object_vert.glsl", "..\\..\\res\\billboard_frag.glsl");
        dir_light_gizmo.initialize();
    }

    bool is_reloading = false;

    void hot_reload() {
        is_reloading = true;
#ifndef NDEBUG
        printf("\n[INFO] Using hot reload...\n");
#endif
        over_shut();
        over_init();
#ifndef NDEBUG
        printf("[INFO] Hot reload complete.\n");
#endif
        is_reloading = false;
    }

    int mode = 0;
    bool flashlight = false;
	virtual void update() override {
        if(is_reloading) return;
        //Take care of input.
        input();
        //Create transformations.
        camera.aspect = static_cast<float>(window.width) / static_cast<float>(window.height);
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 projection = camera.get_projection_matrix();
        //Render model.
        box_shader.enable();
        box_shader.set_3_floats("view_pos", camera.position);
        //Point light.
        if(!flashlight) {
            for (unsigned int i = 0; i < point_lights_amount; i++)
                lights[i].light.render(box_shader, i);
            box_shader.set_int("num_point_lights", 4);
        } else box_shader.set_int("num_point_lights", 0);
        //Directional light.
        dir_light.direction = glm::vec3(
            glm::rotate(glm::mat4(1.f), glm::radians(0.1f), glm::vec3(1.f, 0.f, 0.f)) *
            glm::vec4(dir_light.direction, 1.f)
        );
        if(!flashlight) {
            //Directional light.
            dir_light.render(box_shader);
            box_shader.set_int("num_spot_lights", 0);
        } else {
            //Spot light.
            spot_light.position = camera.position;
            spot_light.direction = camera.forward;
            spot_light.render(box_shader, 0);
            box_shader.set_int("num_spot_lights", 1);
        }
        box_shader.set_mat4("projection", projection);
        box_shader.set_mat4("view", view);
        box_shader.set_int("render_mode", mode);
        model.render(box_shader);
        //Render light.
        light_shader.enable();
        light_shader.set_mat4("projection", projection );
        light_shader.set_mat4("view", view);
        light_shader.set_int("render_mode", mode);
        if (!flashlight)
            for (unsigned int i = 0; i < point_lights_amount; i++)
                lights[i].render(light_shader);
        //Gizmo.
        gizmo_shader.set_mat4("projection", projection);
        gizmo_shader.set_mat4("view", view);
        gizmo_shader.set_4_floats("albedo_color", dir_light.color);
        gizmo_shader.set_int("render_mode", mode);
        dir_light_gizmo.position = glm::vec3(0.f, -dir_light.direction.y, -dir_light.direction.z * 1.5);
        dir_light_gizmo.render(gizmo_shader);
        //UI rendering.
        UI::setTheme();
        UI::draw(this, &camera);
    }

    bool clicked = false;
    bool clicked_now = true;
    void input() {
        //On 'Esc' close app.
        if(Keyboard::key_state(KeyCode::ESCAPE) || main_j.button_state(JoystickControls::J_HOME))
            App::shutdown();
        //Hot reloading.
        if(Keyboard::key_state(KeyCode::R)) hot_reload();
        //Default mode.
        if(Keyboard::key_down(KeyCode::KEY_1) || main_j.button_state(JoystickControls::TRIGGER_LEFT)) { mode = 0;
            a.play();
            Renderer::switch_mode(RenderMode::DEFAULT);
        }
        //Wireframe mode.
        if(Keyboard::key_down(KeyCode::KEY_2) || main_j.button_state(JoystickControls::TRIGGER_RIGHT)) { mode = 0;
            Renderer::switch_mode(RenderMode::WIREFRAME);
        }
        //UV mode.
        if(Keyboard::key_down(KeyCode::KEY_3)) { mode = 1;
            Renderer::switch_mode(RenderMode::DEFAULT);
        }
        //Normal mode.
        if(Keyboard::key_down(KeyCode::KEY_4)) { mode = 2;
            Renderer::switch_mode(RenderMode::DEFAULT);
        }
        //Movement.
        float j_x = main_j.axis_state(JoystickControls::AXES_LEFT_STICK_X);
        float j_y = main_j.axis_state(JoystickControls::AXES_LEFT_STICK_Y);
        float mouse_dy = static_cast<float>(Mouse::get_wheel_dy());
        //Check if RMB is pressed.
        if(Mouse::button_down(1)) clicked = !clicked;
        if(clicked) {
            window.set_param(W_CURSOR, C_DISABLED);
            //Position changes.
            // F/B movement.
            if(Keyboard::key_state(KeyCode::W) || j_y <= -0.5f)
                camera.position += camera.forward * (App::delta_time * 2.5f);
            if(Keyboard::key_state(KeyCode::S) || j_y >= 0.5f)
                camera.position -= camera.forward * (App::delta_time * 2.5f);
            // R/L movement.
            if(Keyboard::key_state(KeyCode::D) || j_x >= 0.5f)
                camera.position += camera.right * (App::delta_time * 2.5f);
            if(Keyboard::key_state(KeyCode::A) || j_x <= -0.5f)
                camera.position -= camera.right * (App::delta_time * 2.5f);
            // U/D movement.
            if(Keyboard::key_state(KeyCode::SPACE) || main_j.button_state(JoystickControls::DPAD_UP))
                camera.position += camera.up * (App::delta_time * 2.5f);
            if(Keyboard::key_state(KeyCode::LEFT_SHIFT) || main_j.button_state(JoystickControls::DPAD_DOWN))
                camera.position -= camera.up * (App::delta_time * 2.5f);
            //Camera rotation.
            double dx = Mouse::get_cursor_dx(), dy = Mouse::get_cursor_dy();
            if(clicked_now) dx = dy = 0;
            if(dx != 0 || dy != 0) rotate_camera(dx, dy);
            else if(main_j.is_present()) {
                dx = main_j.axis_state(JoystickControls::AXES_RIGHT_STICK_X);
                dy = -main_j.axis_state(JoystickControls::AXES_RIGHT_STICK_Y);
                if(dx >= 0.5 || dy >= 0.5 || dx <= -0.5 || dy <= -0.5)
                    rotate_camera(dx, dy);
            }
            //Move with mouse wheel.
            if(mouse_dy != 0)
                camera.position += camera.forward * (App::delta_time * mouse_dy * 2.5f);
            //Update UI.
            UI::update_pos(&camera);
            clicked_now = false;
        } else {
            window.set_param(W_CURSOR, C_NONE);
            //Camera zoom.
            camera.fov -= mouse_dy;
            if(camera.fov < 1.f) camera.fov = 180.f;
            else if(camera.fov > 180.f) camera.fov = 1.f;
            clicked_now = true;
        }
        //Lighting.
        if(Keyboard::key_down(KeyCode::F) || main_j.button_state(JoystickControls::BTN_UP)) {
            flashlight = !flashlight;
            if(flashlight) Renderer::set_clear_color(glm::vec3(0.f, 0.f, 0.f));
            else Renderer::set_clear_color(glm::vec3(0.15f, 0.15f, 0.15f));
            UI::update_bg();
        }
        //Update joystick.
        main_j.update();
    }

    void rotate_camera(double dz, double dy) {
        camera.rotation.z += static_cast<float>(dz);
        if(camera.rotation.z >= 360) camera.rotation.z -= 360;
        if(camera.rotation.z <= -360) camera.rotation.z += 360;
        camera.rotation.y += static_cast<float>(dy);
        if(camera.rotation.y > 89.f) camera.rotation.y = 89.f;
        else if(camera.rotation.y < -89.f) camera.rotation.y = -89.f;
        camera.update_vectors();
    }

    virtual void on_shutdown() override {
        over_shut();
        AudioManager::remove();
        window.ui_shutdown();
        Utils::write_file("window_config.ini",
            "[Window][Hello XEngine!]\nSize=" + std::to_string(window.width) + "," + std::to_string(window.height));
    }

    void over_shut() {
        model.remove();
        box_shader.remove();
        //dir_light_gizmo.remove();
        //gizmo_shader.remove();
        for(unsigned int i = 0; i < point_lights_amount; i++)
            lights[i].remove();
        light_shader.remove();
    }

};

int main() {
    //Entire application startup.
    //Get saved window size.
    std::vector<std::string> w_size = { "1300", "800" };
    std::string file = Utils::read_from_file("window_config.ini");
    if (file != "") {
        std::vector<std::string> content = Utils::split_str(&file, '\n');
        w_size = Utils::split_str(&(Utils::split_str(&content[1], '=')[1]), ',');
    }
    //Create instance of app.
    auto editor = std::make_unique<EditorApp>();
    int returnC = editor->start(std::stoi(w_size[0]), std::stoi(w_size[1]), "Hello XEngine!");
    return 0;
}