#include <imgui.h>
#include <imgui_internal.h>
#include <string>

#include <xengine/rendering/renderer.hpp>
#include <xengine/rendering/camera.hpp>
#include <xengine/enviroment.hpp>
#include <xengine/io/os.hpp>

#include "components/light_cube.hpp"
#include "ui.hpp"
#include <xengine/physics/rigidbody.hpp>

void UI::setTheme() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Color palette
    ImVec4 _black = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    ImVec4 _white = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    ImVec4 _grey = ImVec4(0.60f, 0.60f, 0.60f, 0.35f);
    ImVec4 _dark = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    ImVec4 _darkgrey = ImVec4(0.23f, 0.23f, 0.23f, 0.35f);
    ImVec4 _theme_clr = ImVec4(1.f, 0.5f, 0.f, 1.f);
    ImVec4 _light_theme_clr = ImVec4(1.f, 0.6f, 0.f, 1.f);
    ImVec4 _dark_theme_clr = ImVec4(0.7f, 0.3f, 0.f, 1.f);
    ImVec4 _lighgrey = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);

    // Color array
    style.Colors[ImGuiCol_Text] = _white;
    style.Colors[ImGuiCol_TextDisabled] = _grey;
    style.Colors[ImGuiCol_WindowBg] = _dark;
    style.Colors[ImGuiCol_ChildBg] = _dark;
    style.Colors[ImGuiCol_PopupBg] = _dark;
    style.Colors[ImGuiCol_Border] = _grey;
    style.Colors[ImGuiCol_BorderShadow] = _black;
    style.Colors[ImGuiCol_FrameBg] = _darkgrey;
    style.Colors[ImGuiCol_FrameBgHovered] = _dark_theme_clr;
    style.Colors[ImGuiCol_FrameBgActive] = _grey;
    style.Colors[ImGuiCol_TitleBg] = _darkgrey;
    style.Colors[ImGuiCol_TitleBgActive] = _darkgrey;
    style.Colors[ImGuiCol_TitleBgCollapsed] = _darkgrey;
    style.Colors[ImGuiCol_MenuBarBg] = _darkgrey;
    style.Colors[ImGuiCol_ScrollbarBg] = _darkgrey;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = _grey;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = _grey;
    style.Colors[ImGuiCol_CheckMark] = _lighgrey;
    style.Colors[ImGuiCol_SliderGrab] = _lighgrey;
    style.Colors[ImGuiCol_SliderGrabActive] = _white;
    style.Colors[ImGuiCol_Button] = _darkgrey;
    style.Colors[ImGuiCol_ButtonHovered] = _grey;
    style.Colors[ImGuiCol_ButtonActive] = _darkgrey;
    style.Colors[ImGuiCol_Header] = _darkgrey;
    style.Colors[ImGuiCol_HeaderHovered] = _grey;
    style.Colors[ImGuiCol_HeaderActive] = _grey;
    style.Colors[ImGuiCol_Separator] = _grey;
    style.Colors[ImGuiCol_SeparatorHovered] = _theme_clr;
    style.Colors[ImGuiCol_SeparatorActive] = _light_theme_clr;
    style.Colors[ImGuiCol_ResizeGrip] = _darkgrey;
    style.Colors[ImGuiCol_ResizeGripHovered] = _theme_clr;
    style.Colors[ImGuiCol_ResizeGripActive] = _light_theme_clr;
    style.Colors[ImGuiCol_Tab] = _theme_clr;
    style.Colors[ImGuiCol_TabHovered] = _light_theme_clr;
    style.Colors[ImGuiCol_TabActive] = _theme_clr;
    style.Colors[ImGuiCol_TabUnfocused] = _grey;
    style.Colors[ImGuiCol_TabUnfocused] = _grey;
    style.Colors[ImGuiCol_TabUnfocusedActive] = _grey;
    style.Colors[ImGuiCol_DockingPreview] = _theme_clr;
    style.Colors[ImGuiCol_DockingEmptyBg] = _theme_clr;
    style.Colors[ImGuiCol_PlotLines] = _white;
    style.Colors[ImGuiCol_PlotLinesHovered] = _grey;
    style.Colors[ImGuiCol_PlotHistogram] = _white;
    style.Colors[ImGuiCol_PlotHistogramHovered] = _grey;
    style.Colors[ImGuiCol_TableHeaderBg] = _dark;
    style.Colors[ImGuiCol_TableBorderStrong] = _darkgrey;
    style.Colors[ImGuiCol_TableBorderLight] = _grey;
    style.Colors[ImGuiCol_TableRowBg] = _black;
    style.Colors[ImGuiCol_TableRowBgAlt] = _white;
    style.Colors[ImGuiCol_TextSelectedBg] = _darkgrey;
    style.Colors[ImGuiCol_DragDropTarget] = _darkgrey;
    style.Colors[ImGuiCol_NavHighlight] = _grey;
    style.Colors[ImGuiCol_NavWindowingHighlight] = _grey;
    style.Colors[ImGuiCol_NavWindowingDimBg] = _grey;
    style.Colors[ImGuiCol_ModalWindowDimBg] = _grey;
}

float bg_cols[] = { 0.15f, 0.15f, 0.15f };
float cube_cols[] = { 1.f, 1.f, 1.f };
float pos[] = { 0.f, 0.f, 0.f };
float rot[] = { 0.f, 0.f, 0.f };
void UI::update_pos(XEngine::Camera* t_camera) {
    glm::vec3 val = t_camera->position;
    pos[0] = val.x;
    pos[1] = val.y;
    pos[2] = val.z;
    rot[0] = t_camera->rotation.x;
    rot[1] = t_camera->rotation.y;
    rot[2] = t_camera->rotation.z;
}

static bool is_equal(float x, float y, float z, glm::vec3 t_a2) {
    return x == t_a2.x && y == t_a2.y && z == t_a2.z;
}

bool is_editor_open = true;
bool is_demo_win_open = false;

static glm::vec3 DragFloat3(const char* t_name, glm::vec3 t_value,
    float t_add_val = 1.0f, float t_min = -1024.f, float t_max = 1024.f) {
    float conversion_array[3] = { t_value.x, t_value.y, t_value.z };
    ImGui::DragFloat3(t_name, conversion_array, t_add_val, t_min, t_max);
    return glm::vec3(conversion_array[0], conversion_array[1], conversion_array[2]);
}

static glm::vec4 DragFloat4(const char* t_name, glm::vec4 t_value,
    float t_add_val = 1.0f, float t_min = -1024.f, float t_max = 1024.f) {
    float conversion_array[4] = { t_value.x, t_value.y, t_value.z, t_value.w };
    ImGui::DragFloat4(t_name, conversion_array, t_add_val, t_min, t_max);
    return glm::vec4(conversion_array[0], conversion_array[1], conversion_array[2], conversion_array[3]);
}

static void draw_editor(XEngine::App* t_app, XEngine::Camera* t_camera, Cube* t_cube, Transform* t_model) {
    //Basic values and info.
    ImGui::Text(("FPS: " + std::to_string(t_app->fps)).c_str());
    if(ImGui::CollapsingHeader("Window")) {
        bool vsync = t_app->window.get_param_b(XEngine::W_VSYNC);
        ImGui::Checkbox("V-Sync", &vsync);
        t_app->window.set_param(XEngine::W_VSYNC, vsync);
        XEngine::Renderer::set_clear_color(DragFloat3("BG Color", XEngine::Renderer::get_clear_color(), 0.01f, 0.f, 1.f));
    }
    //Enviroment.
    if(ImGui::CollapsingHeader("Enviroment")) {
        XEngine::Enviroment::gravity = DragFloat3("Gravity", XEngine::Enviroment::gravity);
    }
    //Object redactor.
    if(ImGui::CollapsingHeader("Objects")) {
        if(ImGui::TreeNode("Camera")) {
            ImGui::Text("Transform data");
            ImGui::Text("Instances: 0"); //Currently camera can't be instanced, so instances amount is hard-coded.
            ImGui::Text("Components (1)"); //Currently camera isn't a component, so components amount is hard-coded.
            if(ImGui::TreeNode("Camera")) {
                ImGui::Checkbox("Prespective", &(t_camera->is_perspective));
                ImGui::SliderFloat("Far plane", &t_camera->far_plane, 0.1f, 1000.f);
                ImGui::SliderFloat("Near plane", &t_camera->near_plane, 0.1f, 10.f);
                ImGui::SliderFloat("FOV", &t_camera->fov, 0.1f, 180.f);
                ImGui::DragFloat3("Position", pos);
                ImGui::DragFloat3("Rotation", rot);
                t_camera->position = glm::vec3(pos[0], pos[1], pos[2]);
                if (!is_equal(rot[0], rot[1], rot[2], glm::vec3(t_camera->rotation.x, t_camera->rotation.y, t_camera->rotation.z))) {
                    t_camera->rotation = glm::vec4(rot[0], rot[1], rot[2], 1.f);
                    t_camera->update_vectors();
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Lights")) {
            ImGui::Text("Transform data");
            ImGui::Text(("Instances: " + std::to_string(t_cube->instances_amount())).c_str());
            ImGui::Text(("Components (" + std::to_string(t_cube->components_amount()) + ")").c_str());
            if(ImGui::TreeNode("LightSource")) {
                ImGui::Text(("PointLight global ID: " + std::to_string(LightSource::global_id)).c_str());
                t_cube->color = DragFloat4("Color", t_cube->color, 0.01f, 0.f, 1.f);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Model")) {
            ImGui::Text("Transform data");
            ImGui::Text(("Instances: " + std::to_string(t_model->instances_amount())).c_str());
#ifdef XENGINE_IO
            if(ImGui::Button("Load model")) t_model->load_model(XEngine::OS::open_file_dialog(t_model->get_model_path()));
#endif
            t_model->position = DragFloat3("Position", t_model->position);
            t_model->rotation = DragFloat4("Rotation", t_model->rotation);
            t_model->size = DragFloat3("Scale", t_model->size);
            ImGui::Text(("Components (" + std::to_string(t_model->components_amount()) + ")").c_str());
            if(ImGui::TreeNode("Rigidbody")) {
                ImGui::Checkbox("Use gravity", &t_model->get_component<Rigidbody>().use_gravity);
                float mass = t_model->get_component<Rigidbody>().mass;
                ImGui::DragFloat("Mass", &mass, 0.1f, 0.f);
                t_model->get_component<Rigidbody>().mass = mass;
                t_model->get_component<Rigidbody>().acceleration = DragFloat3("Acceleration",
                    t_model->get_component<Rigidbody>().acceleration, 0.1f);
                t_model->get_component<Rigidbody>().velocity = DragFloat3("Velocity",
                    t_model->get_component<Rigidbody>().velocity, 0.1f);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
    }
    //Controls tab.
    if(ImGui::CollapsingHeader("Controls")) {
        ImGui::Text("Without RMB");
        ImGui::TextWrapped("Scrollwheel - Zoom in/out");
        ImGui::Text("With RMB");
        ImGui::TextWrapped("Scrollwheel - Move forwards/backwards, relative to camera");
        ImGui::TextWrapped("WASD - Movement scheme");
        ImGui::TextWrapped("Left Shift - Move down");
        ImGui::TextWrapped("Spacebar - Move up");
        ImGui::TextWrapped("1 - Default rendering (Base)");
        ImGui::TextWrapped("2 - Wireframe rendering");
        ImGui::TextWrapped("3 - UV rendering");
        ImGui::TextWrapped("4 - Normal rendering");
        ImGui::TextWrapped("V - Play sample sound");
        ImGui::TextWrapped("T - Add velocity to rigidbody (model)");
        ImGui::TextWrapped("F - Spot/Point light mode");
        ImGui::TextWrapped("R - Hot resource reload");
    }
    ImGui::End();
}

static void setup_dock(XEngine::App* t_app) {
    //Variables.
    bool open = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoBackground;
    //Do fullscreen.
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("XEditor UI", &open, window_flags);
    ImGui::PopStyleVar(3);
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGuiID dockspace_id = ImGui::GetID("XEditor UI");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    //Start menu.
    if(ImGui::BeginMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("New Project (Ctrl+N)")) { }
            if(ImGui::MenuItem("Open Project (Ctrl+O)")) { }
            ImGui::Separator();
            if(ImGui::MenuItem("Save (Ctrl+S)")) { }
            if(ImGui::MenuItem("Save as... (Alt+S)")) {  }
            ImGui::Separator();
            if(ImGui::MenuItem("Exit (Esc)"))
                (*t_app).shutdown();
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Hot reload (R)")) {}
            if (ImGui::MenuItem("Undo (Ctrl+Z)")) {}
            if (ImGui::MenuItem("Redo (Ctrl+Shift+Z)")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Project settings")) {}
            if (ImGui::MenuItem("Preferences")) {}
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("View")) {
            ImGui::Text("Layouts");
            if(ImGui::MenuItem("Default")) {}
            if(ImGui::MenuItem("Wide")) {}
            if(ImGui::MenuItem("Tall")) {}
            ImGui::Separator();
            if(ImGui::BeginMenu("Windows")) {
                if(ImGui::MenuItem("Editor")) is_editor_open = true;
                if(ImGui::MenuItem("ImGui Demo")) is_demo_win_open = true;
                if(ImGui::MenuItem("Perfomance")) {}
                if(ImGui::MenuItem("Files")) {}
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if(ImGui::MenuItem("Reset (Ctrl+R)")) ImGui::ClearIniSettings();
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Build")) {
            if(ImGui::MenuItem("Compile project (Ctrl+B)")) {}
            if(ImGui::MenuItem("Build settings (Ctrl+Shift+B)")) {}
            if(ImGui::MenuItem("Delete last build (Alt+B)")) {}
            ImGui::Separator();
            if(ImGui::MenuItem("Publish... (Ctrl+P)")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Packages")) {
            if(ImGui::MenuItem("View connected (Ctrl+T)")) {}
            ImGui::Separator();
            if(ImGui::MenuItem("Add package... (Ctrl+Shift+T)")) {}
            if(ImGui::MenuItem("Create package... (Ctrl+G)")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();
}

void UI::draw(UIEditorData t_data) {
    //Setup.
    setup_dock(t_data.t_app);
    //Draw ImGui.
    if(is_demo_win_open) ImGui::ShowDemoWindow(&is_demo_win_open);
    if(is_editor_open) {
        ImGui::Begin("Editor", &is_editor_open);
        draw_editor(t_data.t_app, t_data.t_camera, t_data.t_cube, t_data.t_model);
    }
}