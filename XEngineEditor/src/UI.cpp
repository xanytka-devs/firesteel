#include <imgui.h>
#include "UI.hpp"

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