#include <iostream>
#include <memory>
#include <imgui.h>
#include <imgui_internal.h>

#include <XEngine/App.hpp>
#include <XEngine/Input/Input.hpp>
#include "UI.hpp"

class EditorApp : public XEngine::App {

    bool cameraPanStarted = false;
    glm::vec2 initialPanPos{ 0, 0 };

    virtual void onInitialized() override {
        UI::setTheme();
    }

    virtual void update() override {
        checkInputs();
    }
    virtual void onUIDraw() override {
        //Setup.
        setupDock();
        camPosition[0] = baseCamera.getPosition().x;
        camPosition[1] = baseCamera.getPosition().y;
        camPosition[2] = baseCamera.getPosition().z;
        camRotation[0] = baseCamera.getRotation().x;
        camRotation[1] = baseCamera.getRotation().y;
        camRotation[2] = baseCamera.getRotation().z;
        //Draw ImGui.
        //Editor main
        ImGui::Begin("Editor");
        ImGui::Text("General Testing");
        ImGui::ColorEdit3("Bg Color", bgColor);
        ImGui::Text("Matrix Operations");
        ImGui::DragFloat3("Position", position, -1.f, 1.f);
        ImGui::DragFloat3("Rotation", rotation, 0.f, 360.f);
        ImGui::DragFloat3("Scale", scale, 0.f, 2.f);
        ImGui::End();
        //Editor camera
        ImGui::Begin("Camera");
        ImGui::Text("Camera");
        if(ImGui::DragFloat3("Camera pos", camPosition, -10.f, 10.f))
            baseCamera.setPosition(glm::vec3(camPosition[0], camPosition[1], camPosition[2]));
        if(ImGui::DragFloat3("Camera rot", camRotation, 0, 360.f))
            baseCamera.setRotation(glm::vec3(camRotation[0], camRotation[1], camRotation[2]));
        if(ImGui::Checkbox("Prespective", &camIsPresp))
            baseCamera.setProjectionMode(camIsPresp ?
                XEngine::Camera::ProjectionMode::Perspective : XEngine::Camera::ProjectionMode::Ortographic);
        if(ImGui::SliderFloat("Far plane", &baseCamera.farClipPlane, 0.1f, 1000.f)) baseCamera.setFarClipPlane(baseCamera.farClipPlane);
        if(ImGui::SliderFloat("Near plane", &baseCamera.nearClipPlane, 0.1f, 1000.f)) baseCamera.setNearClipPlane(baseCamera.nearClipPlane);
        if(ImGui::SliderFloat("FOV", &baseCamera.fieldOfView, 0.1f, 120.f)) baseCamera.setFieldOfView(baseCamera.fieldOfView);
        ImGui::End();
        //Editor light
        ImGui::Begin("Light");
        ImGui::Text("Light Source");
        ImGui::DragFloat3("LS position", lightSourcePos, -1.f, 1.f);
        ImGui::ColorEdit3("LS color", lightSourceColor);
        ImGui::SliderFloat("Ambient factor", &ambientFactor, 0.f, 1.f);
        ImGui::SliderFloat("Diffuse factor", &diffuseFactor, 0.f, 1.f);
        ImGui::SliderFloat("Specular factor", &specularFactor, 0.f, 1.f);
        ImGui::SliderFloat("Shininess", &shininess, 1.f, 128.f);
        ImGui::SliderFloat("Emission factor", &emission, 0.f, 2.f);
        ImGui::ColorEdit3("Emission color", emissionColor);
        ImGui::End();
    }

    void setupDock() {
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
                    App::exit();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

    void checkInputs() {
        //Instance vectors.
        glm::vec3 movDelta{ 0, 0, 0 };
        glm::vec3 rotDelta{ 0, 0, 0 };
        //Check for movment keys.
        if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_W))
            movDelta.x += 0.05f;
        if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_S))
            movDelta.x -= 0.05f;
        if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_A))
            movDelta.y -= 0.05f;
        if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_D))
            movDelta.y += 0.05f;
        //Check for rotation keys.
        if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_UP))
            rotDelta.y -= 0.5f;
        if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_DOWN))
            rotDelta.y += 0.5f;
        if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_RIGHT))
            rotDelta.z -= 0.5f;
        if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_LEFT))
            rotDelta.z += 0.5f;
        //Check for mouse changed.
        if (XEngine::Input::isMouseDown(XEngine::MouseButton::MOUSE_BUTTON_RIGHT)) {
            glm::vec2 newMousePos = glm::vec2(0, 0);
            if (!cameraPanStarted) {
                initialPanPos = App::getCursorPosition();
                cameraPanStarted = true;
            }
            newMousePos = App::getCursorPosition();
            if (XEngine::Input::isMouseDown(XEngine::MouseButton::MOUSE_BUTTON_LEFT)) {
                baseCamera.moveRight((initialPanPos.x - newMousePos.x) / 100.f);
                baseCamera.moveUp((initialPanPos.y - newMousePos.y) / 100.f);
            }
            else {
                rotDelta.z += (initialPanPos.x - newMousePos.x) / 5.f;
                rotDelta.y -= (initialPanPos.y - newMousePos.y) / 5.f;
            }
            initialPanPos = newMousePos;
        }
        else cameraPanStarted = false;
        //Move and rotate.
        baseCamera.moveRot(movDelta, rotDelta);

    }

};

int main() {

	auto app = std::make_unique<EditorApp>();

	int returnC = app->start(1024, 768, "Hello XEngine!");

	return returnC;

}