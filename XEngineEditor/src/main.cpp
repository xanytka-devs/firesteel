#include <iostream>
#include <memory>
#include <imgui.h>

#include <XEngine/App.hpp>
#include <XEngine/Input.hpp>

class EditorApp : public XEngine::App {
	
    bool cameraPanStarted = false;
    glm::vec2 initialPanPos{ 0, 0 };

	virtual void update() override {

        checkInputs();
    }
	virtual void onUIDraw() override {

        camPosition[0] = baseCamera.getPosition().x;
        camPosition[1] = baseCamera.getPosition().y;
        camPosition[2] = baseCamera.getPosition().z;
        camRotation[0] = baseCamera.getRotation().x;
        camRotation[1] = baseCamera.getRotation().y;
        camRotation[2] = baseCamera.getRotation().z;

        //Draw ImGui.
        ImGui::Begin("Editor");
        ImGui::Text("General Testing");
        ImGui::ColorEdit3("Bg Color", App::bgColor);
        ImGui::Text("Matrix Operations");
        ImGui::SliderFloat3("Position", position, -1.f, 1.f);
        ImGui::SliderFloat3("Rotation", rotation, 0.f, 360.f);
        ImGui::SliderFloat3("Scale", scale, 0.f, 2.f);
        ImGui::Text("Camera");
        if (ImGui::SliderFloat3("Camera pos", camPosition, -10.f, 10.f))
            baseCamera.setPosition(glm::vec3(camPosition[0], camPosition[1], camPosition[2]));
        if (ImGui::SliderFloat3("Camera rot", camRotation, 0, 360.f))
            baseCamera.setRotation(glm::vec3(camRotation[0], camRotation[1], camRotation[2]));
        ImGui::Checkbox("Prespective", &camIsPresp);
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
        if(XEngine::Input::isMouseDown(XEngine::MouseButton::MOUSE_BUTTON_RIGHT)) {
            if(!cameraPanStarted) {
                cameraPanStarted = true;
                glm::vec2 initialMousePos = App::getCursorPosition();
            }
            glm::vec2 newMousePos = App::getCursorPosition();
            if (XEngine::Input::isMouseDown(XEngine::MouseButton::MOUSE_BUTTON_LEFT)) {
                baseCamera.moveRight((initialPanPos.x - newMousePos.x) / 100.f);
                baseCamera.moveUp((initialPanPos.y - newMousePos.y) / 100.f);
            } else {
                rotDelta.z += (initialPanPos.x - newMousePos.x) / 5.f;
                rotDelta.y += (initialPanPos.y - newMousePos.y) / 5.f;
            }
            initialPanPos = newMousePos;
        } else cameraPanStarted = false;
        //Move and rotate.
        baseCamera.moveRot(movDelta, rotDelta);

    }

};

int main() {

	auto app = std::make_unique<EditorApp>();
	int returnC = app->start(1024, 768, "Hello XEngine!");
	return returnC;

}