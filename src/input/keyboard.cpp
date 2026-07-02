#include <firesteel/input/keyboard.hpp>

namespace Firesteel {
	bool Keyboard::mKeys[GLFW_KEY_LAST] = { 0 };
	bool Keyboard::mKeysChanged[GLFW_KEY_LAST] = { 0 };
	bool Keyboard::mAnyKeyPressed = false;

	void Keyboard::keyCallback(GLFWwindow* tWindow, int tKey, int tScanCode, int tAction, int tMods) {
		//Check action.
		if (tKey == -1) return;
		if (tAction != GLFW_RELEASE) {
			mAnyKeyPressed = true;
			if (!mKeys[tKey]) mKeys[tKey] = true;
		}
		else {
			mAnyKeyPressed = false;
			mKeys[tKey] = false;
		}
		if (tAction == GLFW_REPEAT) mAnyKeyPressed = false;
		//Detect if key is pressed continuously.
		mKeysChanged[tKey] = tAction != GLFW_REPEAT;
	}

	bool Keyboard::getKey(const int tKey) {
		return mKeys[tKey];
	}

	bool Keyboard::keyChanged(const int tKey) {
		bool output = mKeysChanged[tKey];
		mKeysChanged[tKey] = false;
		return output;
	}

	bool Keyboard::keyUp(const int tKey) {
		return !mKeys[tKey] && keyChanged(tKey);
	}

	bool Keyboard::keyDown(const int tKey) {
		return mKeys[tKey] && keyChanged(tKey);
	}
}