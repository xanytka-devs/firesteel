#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "TUI.hpp"

namespace XEngine::UI {

    void TUI::init(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;

        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
    }
    void TUI::shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void TUI::update() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    void TUI::draw() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void TUI::SetTheme() {
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

    void TUI::ShowExampleAppDockSpace(bool* p_open) {
        // If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
        // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
        // In this specific demo, we are not using DockSpaceOverViewport() because:
        // - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
        // - we allow the host window to have padding (when opt_padding == true)
        // - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
        // TL;DR; this demo is more complicated than what you would normally use.
        // If we removed all the options we are showcasing, this demo would become:
        //     void ShowExampleAppDockSpace()
        //     {
        //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        //     }

        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", p_open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Options")) {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                ImGui::Separator();

                if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                    *p_open = false;
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

}