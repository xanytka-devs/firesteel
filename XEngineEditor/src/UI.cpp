#include <imgui.h>

#include "UI.hpp"
#include <imgui_internal.h>

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

void UI::Draw(XEditor::EditorApp* app) {
    //Setup.
    setupDock(app);
    (*app).camPosition[0] = (*app).baseCamera.getPosition().x;
    (*app).camPosition[1] = (*app).baseCamera.getPosition().y;
    (*app).camPosition[2] = (*app).baseCamera.getPosition().z;
    (*app).camRotation[0] = (*app).baseCamera.getRotation().x;
    (*app).camRotation[1] = (*app).baseCamera.getRotation().y;
    (*app).camRotation[2] = (*app).baseCamera.getRotation().z;
    //Draw ImGui.
    //Editor main
    ImGui::Begin("Editor");
    ImGui::Text("General Testing");
    ImGui::ColorEdit3("Bg Color", (*app).bgColor);
    ImGui::Text("Matrix Operations");
    ImGui::DragFloat3("Position", (*app).position, -1.f, 1.f);
    ImGui::DragFloat3("Rotation", (*app).rotation, 0.f, 360.f);
    ImGui::DragFloat3("Scale", (*app).scale, 0.f, 2.f);
    ImGui::End();
    //Editor camera
    ImGui::Begin("Camera");
    ImGui::Text("Camera");
    if (ImGui::DragFloat3("Camera pos", (*app).camPosition, -10.f, 10.f))
        (*app).baseCamera.setPosition(glm::vec3((*app).camPosition[0], (*app).camPosition[1], (*app).camPosition[2]));
    if (ImGui::SliderFloat3("Camera rot", (*app).camRotation, 0, 360.f))
        (*app).baseCamera.setRotation(glm::vec3((*app).camRotation[0], (*app).camRotation[1], (*app).camRotation[2]));
    if (ImGui::Checkbox("Prespective", &(*app).camIsPresp))
        (*app).baseCamera.setProjectionMode((*app).camIsPresp ?
            XEngine::Camera::ProjectionMode::Perspective : XEngine::Camera::ProjectionMode::Ortographic);
    if (ImGui::SliderFloat("Far plane", &(*app).baseCamera.farClipPlane, 0.1f, 1000.f)) (*app).baseCamera.setFarClipPlane((*app).baseCamera.farClipPlane);
    if (ImGui::SliderFloat("Near plane", &(*app).baseCamera.nearClipPlane, 0.1f, 1000.f)) (*app).baseCamera.setNearClipPlane((*app).baseCamera.nearClipPlane);
    if (ImGui::SliderFloat("FOV", &(*app).baseCamera.fieldOfView, 0.1f, 120.f)) (*app).baseCamera.setFieldOfView((*app).baseCamera.fieldOfView);
    ImGui::End();
    //Editor light
    ImGui::Begin("Light");
    ImGui::Text("Light Source");
    ImGui::DragFloat3("LS position", (*app).lightSourcePos, -1.f, 1.f);
    ImGui::ColorEdit3("LS color", (*app).lightSourceColor);
    ImGui::SliderFloat("Ambient factor", &(*app).ambientFactor, 0.f, 1.f);
    ImGui::SliderFloat("Diffuse factor", &(*app).diffuseFactor, 0.f, 1.f);
    ImGui::SliderFloat("Specular factor", &(*app).specularFactor, 0.f, 1.f);
    ImGui::SliderFloat("Shininess", &(*app).shininess, 1.f, 128.f);
    ImGui::SliderFloat("Emission factor", &(*app).emission, 0.f, 2.f);
    ImGui::ColorEdit3("Emission color", (*app).emissionColor);
    ImGui::End();
}

void setupDock(XEditor::EditorApp* app) {
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
    ImGuiID dockspace_id = ImGui::GetID("XEditor UI");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    //Start menu.
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Project")) {

            }
            if (ImGui::MenuItem("Open Project")) {

            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save")) {

            }
            if (ImGui::MenuItem("Save as...")) {

            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                (*app).exit();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}