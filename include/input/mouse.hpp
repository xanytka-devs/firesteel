#ifndef FS_MOUSE_H
#define FS_MOUSE_H

#include <../include/common.hpp>

namespace Firesteel {
	class Mouse {
	public:
		// Set mouse cursor position.
		static void cursorCallback(GLFWwindow* tWindow, double tX, double tY) {
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
		// Set mouse buttons statuses.
		static void buttonCallback(GLFWwindow* tWindow, int tButton, int tAction, int tMods) {
			mButtons[tButton] = tAction != GLFW_RELEASE
				&& !mButtons[tButton]
				&& tAction != GLFW_REPEAT;
		}
		// Set mouse wheel deltas.
		static void scrollCallback(GLFWwindow* tWindow, double tDx, double tDy) {
			mWheelDX = static_cast<float>(tDx);
			mWheelDY = static_cast<float>(tDy);
		}

		static float getCursorX() { return mX; }
		static float getCursorY() { return mY; }
		static glm::vec2 getCursorPosition() { return glm::vec2(mX, mY); }

		// Get cursor x delta.
		static float getCursorDX() {
			float _dx = mDX;
			mDX = 0;
			return _dx;
		}
		// Get cursor y delta.
		static float getCursorDY() {
			float _dy = mDY;
			mDY = 0;
			return _dy;
		}

		// Get mouse wheel x delta.
		static float getWheelDX() {
			float w_dx = mWheelDX;
			mWheelDX = 0;
			return w_dx;
		}
		// Get mouse wheel y delta.
		static float getWheelDY() {
			float w_dy = mWheelDY;
			mWheelDY = 0;
			return w_dy;
		}

		// Gets current state of given mouse button.
		static bool getButton(const int tButton) {
			return mButtons[tButton];
		}
		// Has mouse button changed?
		static bool buttonChanged(const int tButton) {
			bool output = mButtons[tButton];
			mButtons[tButton] = false;
			return output;
		}
		// Is mouse button up?
		static bool buttonUp(const int tButton) {
			return !mButtons[tButton] && buttonChanged(tButton);
		}
		// Is mouse button down?
		static bool buttonDown(const int tButton) {
			return mButtons[tButton] && buttonChanged(tButton);
		}
	private:
		static float mX, mY, m_old_x, m_old_y;

		static float mDX, mDY;

		static float mWheelDX, mWheelDY;

		static bool mFirstMove;
		static bool mButtons[];
		static bool mButtonsChanged[];
	};
}

#endif // !FS_MOUSE_H