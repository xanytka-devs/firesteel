#ifndef FS_MOUSE_H
#define FS_MOUSE_H

#include <../include/common.hpp>

namespace Firesteel {
	class Mouse {
	public:
		/// Set mouse cursor position.
		static void cursorCallback(GLFWwindow* t_window, double t_x, double t_y) {
			mX = static_cast<float>(t_x);
			mY = static_cast<float>(t_y);
			//Check if first mouse button.
			if (mFirstMove) {
				m_old_x = static_cast<float>(t_x);
				m_old_y = static_cast<float>(t_y);
				mFirstMove = false;
			}
			//Set deltas.
			mDX = mX - m_old_x;
			mDY = m_old_y - mY;
			//Set old positions
			m_old_x = mX;
			m_old_y = mY;
		}
		/// Set mouse buttons statuses.
		static void buttonCallback(GLFWwindow* t_window, int t_button, int t_action, int t_mods) {
			mButtons[t_button] = t_action != GLFW_RELEASE
				&& !mButtons[t_button]
				&& t_action != GLFW_REPEAT;
		}
		/// Set mouse wheel deltas.
		static void scrollCallback(GLFWwindow* t_window, double t_dx, double t_dy) {
			mWheelDX = static_cast<float>(t_dx);
			mWheelDY = static_cast<float>(t_dy);
		}

		/// Get cursor x.
		static float getCursorX() { return mX; }
		/// Get cursor y.
		static float getCursorY() { return mY; }

		/// Get cursor x delta.
		static float getCursorDX() {
			float _dx = mDX;
			mDX = 0;
			return _dx;
		}
		/// Get cursor y delta.
		static float getCursorDY() {
			float _dy = mDY;
			mDY = 0;
			return _dy;
		}

		/// Get mouse wheel x delta.
		static float getWheelDX() {
			float w_dx = mWheelDX;
			mWheelDX = 0;
			return w_dx;
		}
		/// Get mouse wheel y delta.
		static float getWheelDY() {
			float w_dy = mWheelDY;
			mWheelDY = 0;
			return w_dy;
		}

		/// Gets current state of given mouse button.
		static bool getButton(int t_button) {
			return mButtons[t_button];
		}
		/// Has mouse button changed?
		static bool buttonChanged(int t_button) {
			bool output = mButtons[t_button];
			mButtons[t_button] = false;
			return output;
		}
		/// Is mouse button up?
		static bool buttonUp(int t_button) {
			return !mButtons[t_button] && buttonChanged(t_button);
		}
		/// Is mouse button down?
		static bool buttonDown(int t_button) {
			return mButtons[t_button] && buttonChanged(t_button);
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