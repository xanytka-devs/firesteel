#pragma once

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

		static void setMousePosition(const double& posX, const double& posY) { getInstance().mousePos = glm::vec2(posX, posY); }
		static glm::vec2 getMousePosition() { return getInstance().mousePos; }

		static bool isMouseScrolling() { return getInstance().mouseScroll.y != 0.f || getInstance().mouseScroll.x != 0.f; }
		static bool isMouseScrollingX() { return getInstance().mouseScroll.x != 0.f; }
		static bool isMouseScrollingY() { return getInstance().mouseScroll.y != 0.f; }
		static void setMouseScroll(const double& posX, const double& posY) { getInstance().mouseScroll = glm::vec2(posX, posY); }
		static glm::vec2 getMouseScroll() { return getInstance().mouseScroll; }
	private:
		Input() : mousePos(0, 0), mouseScroll(0, 0) {}

		static Input& getInstance() {
			static Input instance;
			return instance;
		}

		glm::vec2 mousePos;
		glm::vec2 mouseScroll;
		static bool keysPressed[];
		static bool mouseBtnsPressed[];
	};

}