#include <imgui.h>
#include <imgui_internal.h>
#include <string>

#include <xengine/rendering/renderer.hpp>
#include <xengine/rendering/camera.hpp>
#include <xengine/enviroment.hpp>

#include "ui.hpp"

void UI::setTheme() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Color palette
    ImVec4 _black = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    ImVec4 _white = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    ImVec4 _grey = ImVec4(0.60f, 0.60f, 0.60f, 0.35f);
    ImVec4 _dark = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    ImVec4 _darkgrey = ImVec4(0.23f, 0.23f, 0.23f, 0.35f);
    ImVec4 _themeColor = ImVec4(1.f, 0.5f, 0.f, 1.f);
    ImVec4 _lightThemeColor = ImVec4(1.f, 0.6f, 0.f, 1.f);
    ImVec4 _darkThemeColor = ImVec4(0.7f, 0.3f, 0.f, 1.f);
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
    style.Colors[ImGuiCol_FrameBgHovered] = _darkThemeColor;
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
    style.Colors[ImGuiCol_SeparatorHovered] = _themeColor;
    style.Colors[ImGuiCol_SeparatorActive] = _lightThemeColor;
    style.Colors[ImGuiCol_ResizeGrip] = _darkgrey;
    style.Colors[ImGuiCol_ResizeGripHovered] = _themeColor;
    style.Colors[ImGuiCol_ResizeGripActive] = _lightThemeColor;
    style.Colors[ImGuiCol_Tab] = _themeColor;
    style.Colors[ImGuiCol_TabHovered] = _lightThemeColor;
    style.Colors[ImGuiCol_TabActive] = _themeColor;
    style.Colors[ImGuiCol_TabUnfocused] = _grey;
    style.Colors[ImGuiCol_TabUnfocused] = _grey;
    style.Colors[ImGuiCol_TabUnfocusedActive] = _grey;
    style.Colors[ImGuiCol_DockingPreview] = _themeColor;
    style.Colors[ImGuiCol_DockingEmptyBg] = _themeColor;
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

float cols[] = { 0.15f, 0.15f, 0.15f };
float pos[] = { 0.f, 0.f, 0.f };
float rot[] = { 0.f, 0.f, 0.f };
void UI::update_bg() {
    glm::vec4 val = XEngine::Renderer::get_clear_color();
    cols[0] = val.x;
    cols[1] = val.y;
    cols[2] = val.z;
}
void UI::update_pos(XEngine::Camera* t_camera) {
    glm::vec3 val = t_camera->position;
    pos[0] = val.x;
    pos[1] = val.y;
    pos[2] = val.z;
    rot[0] = t_camera->rotation.x;
    rot[1] = t_camera->rotation.y;
    rot[2] = t_camera->rotation.z;
}

bool is_equal(float x, float y, float z, glm::vec3 t_a2) {
    return x == t_a2.x && y == t_a2.y && z == t_a2.z;
}

bool is_editor_open = true;
bool is_demo_win_open = false;

void draw_editor(XEngine::App* t_app, XEngine::Camera* t_camera) {
    //Basic values and info.
    ImGui::Text("Window");
    bool vsync = t_app->window.get_param_b(XEngine::W_VSYNC);
    ImGui::Checkbox("V-Sync", &vsync);
    t_app->window.set_param(XEngine::W_VSYNC, vsync);
    ImGui::Text("General");
    ImGui::Text(("FPS: " + std::to_string(t_app->fps)).c_str());
    ImGui::ColorEdit3("Bg Color", cols);
    XEngine::Renderer::set_clear_color(cols[0], cols[1], cols[2]);
    //Enviroment.
    ImGui::Text("Enviroment");
    glm::vec3 t_g = XEngine::Enviroment::gravity;
    float grav[] = { t_g.x, t_g.y, t_g.z };
    ImGui::DragFloat3("Gravity", grav);
    XEngine::Enviroment::gravity = glm::vec3(grav[0], grav[1], grav[2]);
    //Camera redactor.
    ImGui::Text("Camera");
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
    ImGui::End();
}

void setup_dock(XEngine::App* t_app) {
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
                if(ImGui::MenuItem("Shader view")) {}
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

void UI::draw(XEngine::App* t_app, XEngine::Camera* t_camera) {
    //Setup.
    setup_dock(t_app);
    //Draw ImGui.
    if(is_demo_win_open) ImGui::ShowDemoWindow(&is_demo_win_open);
    if(is_editor_open) {
        ImGui::Begin("Editor", &is_editor_open);
        draw_editor(t_app, t_camera);
    }
}