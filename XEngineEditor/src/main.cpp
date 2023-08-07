#include <iostream>
#include <memory>

#include <XEngine/App.hpp>
#include <XEngine/Input/Input.hpp>
#include <XEngine/ResLoader.hpp>
#include "UI.hpp"

namespace XEditor {
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
            EditorApp* app = this;
            UI::Draw(app);
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
            if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_LEFT_SHIFT))
                movDelta *= 1.5f;
            if (!XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_LEFT_CONTROL))
                movDelta.x += XEngine::Input::getMouseScroll().y;
            else baseCamera.setFieldOfView(baseCamera.fieldOfView + XEngine::Input::getMouseScroll().y * 2);
            //Check for rotation keys.
            if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_UP))
                rotDelta.y -= 0.5f;
            if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_DOWN))
                rotDelta.y += 0.5f;
            if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_RIGHT))
                rotDelta.z += 0.5f;
            if (XEngine::Input::isKeyDown(XEngine::KeyCode::KEY_LEFT))
                rotDelta.z -= 0.5f;
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
}

int main() {

	auto app = std::make_unique<XEditor::EditorApp>();

	int returnC = app->start(1024, 768, "Hello XEngine!");

	return returnC;

}