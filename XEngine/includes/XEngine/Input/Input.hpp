#pragma once

#include <glm/vec2.hpp>

#include "Keys.hpp"

namespace XEngine {

	class Input {
	public:
		static bool isKeyDown(const KeyCode code);
		static void pressKey(const KeyCode code);
		static void releaseKey(const KeyCode code);
		static void toggleKey(const KeyCode code);

		static bool isMouseDown(const MouseButton code);
		static void pressMouseKey(const MouseButton code);
		static void releaseMouseKey(const MouseButton code);
		static void toggleMouseKey(const MouseButton code);

		static void setMousePosition(const glm::vec2& pos) { mousePos = pos; }
		static glm::vec2 getMousePosition() { return mousePos; }

		static bool isMouseScrollingX() { return mouseScroll.x != 0.f; }
		static bool isMouseScrollingY() { return mouseScroll.y != 0.f; }
		static void setMouseScroll(const glm::vec2& pos) { mouseScroll = pos; }
		static glm::vec2 getMouseScroll() { return mouseScroll; }
	private:
		static glm::vec2 mousePos;
		static glm::vec2 mouseScroll;
		static bool keysPressed[];
		static bool mouseBtnsPressed[];
	};

}