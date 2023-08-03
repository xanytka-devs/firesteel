#include "XEngine/Input/Input.hpp"
#include "XEngine/Input/Cursor.hpp"

namespace XEngine {

	bool Input::keysPressed[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};
	bool Input::mouseBtnsPressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1] = {};

	bool Input::isKeyDown(const KeyCode code) {
		return keysPressed[static_cast<size_t>(code)];
	}
	void Input::pressKey(const KeyCode code) {
		keysPressed[static_cast<size_t>(code)] = true;
	}
	void Input::releaseKey(const KeyCode code) {
		keysPressed[static_cast<size_t>(code)] = false;
	}
	void Input::toggleKey(const KeyCode code) {
		keysPressed[static_cast<size_t>(code)] =
			!keysPressed[static_cast<size_t>(code)];
	}

	bool Input::isMouseDown(const MouseButton code) {
		return mouseBtnsPressed[static_cast<size_t>(code)];
	}
	void Input::pressMouseKey(const MouseButton code) {
		mouseBtnsPressed[static_cast<size_t>(code)] = true;
	}
	void Input::releaseMouseKey(const MouseButton code) {
		mouseBtnsPressed[static_cast<size_t>(code)] = false;
	}
	void Input::toggleMouseKey(const MouseButton code) {
		mouseBtnsPressed[static_cast<size_t>(code)] =
			!mouseBtnsPressed[static_cast<size_t>(code)];
	}

}