#include <firesteel/input/mouse.hpp>

namespace Firesteel {
	float Mouse::mX = 0;
	float Mouse::mY = 0;
	float Mouse::m_old_x = 0;
	float Mouse::m_old_y = 0;

	float Mouse::mDX = 0;
	float Mouse::mDY = 0;

	float Mouse::mWheelDX = 0;
	float Mouse::mWheelDY = 0;

	bool Mouse::mFirstMove = true;
	bool Mouse::mButtons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
	bool Mouse::mButtonsChanged[GLFW_MOUSE_BUTTON_LAST] = { 0 };

	void Mouse::cursorCallback(GLFWwindow* tWindow, double tX, double tY) {
		mX = static_cast<float>(tX);
		mY = static_cast<float>(tY);
		//Check if first mouse button.
		if (mFirstMove) {
			m_old_x = static_cast<float>(tX);
			m_old_y = static_cast<float>(tY);
			mFirstMove = false;
		}
		//Set deltas.
		mDX = mX - m_old_x;
		mDY = m_old_y - mY;
		//Set old positions
		m_old_x = mX;
		m_old_y = mY;
	}
	void Mouse::buttonCallback(GLFWwindow* tWindow, int tButton, int tAction, int tMods) {
		if (tButton == -1) return;
		if (tAction != GLFW_RELEASE) {
			if (!mButtons[tButton]) mButtons[tButton] = true;
		}
		else mButtons[tButton] = false;
		mButtonsChanged[tButton] = true;
	}
	void Mouse::scrollCallback(GLFWwindow* tWindow, double tDx, double tDy) {
		mWheelDX = static_cast<float>(tDx);
		mWheelDY = static_cast<float>(tDy);
	}

	float Mouse::getCursorDX() {
		float _dx = mDX;
		mDX = 0;
		return _dx;
	}
	float Mouse::getCursorDY() {
		float _dy = mDY;
		mDY = 0;
		return _dy;
	}

	float Mouse::getWheelDX() {
		float w_dx = mWheelDX;
		mWheelDX = 0;
		return w_dx;
	}
	float Mouse::getWheelDY() {
		float w_dy = mWheelDY;
		mWheelDY = 0;
		return w_dy;
	}

	bool Mouse::getButton(const int tButton) {
		return mButtons[tButton];
	}
	bool Mouse::buttonChanged(const int tButton) {
		bool output = mButtonsChanged[tButton];
		mButtonsChanged[tButton] = false;
		return output;
	}
	bool Mouse::buttonUp(const int tButton) {
		return !mButtons[tButton] && buttonChanged(tButton);
	}
	bool Mouse::buttonDown(const int tButton) {
		return mButtons[tButton] && buttonChanged(tButton);
	}
}