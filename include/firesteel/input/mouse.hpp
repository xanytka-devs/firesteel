#ifndef FS_MOUSE_H
#define FS_MOUSE_H

#include <firesteel/common.hpp>

namespace Firesteel {
	class Mouse {
	public:
		// Set mouse cursor position.
		static void cursorCallback(GLFWwindow* tWindow, double tX, double tY);
		// Set mouse buttons statuses.
		static void buttonCallback(GLFWwindow* tWindow, int tButton, int tAction, int tMods);
		// Set mouse wheel deltas.
		static void scrollCallback(GLFWwindow* tWindow, double tDx, double tDy);

		static float getCursorX() { return mX; }
		static float getCursorY() { return mY; }
		static glm::vec2 getCursorPosition() { return glm::vec2(mX, mY); }

		// Get cursor x delta.
		static float getCursorDX();
		// Get cursor y delta.
		static float getCursorDY();

		// Get mouse wheel x delta.
		static float getWheelDX();
		// Get mouse wheel y delta.
		static float getWheelDY();

		// Gets current state of given mouse button.
		static bool getButton(const int tButton);
		// Has mouse button changed?
		static bool buttonChanged(const int tButton);
		// Is mouse button up?
		static bool buttonUp(const int tButton);
		// Is mouse button down?
		static bool buttonDown(const int tButton);
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