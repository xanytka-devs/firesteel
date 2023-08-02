#pragma once

#include <glm/vec2.hpp>
#include <memory>

#include "Keys.hpp"

namespace XEngine {

	class Input {
	public:
		Input() {
			mousePos = glm::vec2(0, 0);
			mouseScroll = glm::vec2(0, 0);
			if (!i->initialized) {
				i = std::make_unique<Input>();
				initialized = true;
			}
		}
		~Input() {
			i.reset();
		}
		static std::unique_ptr<Input> i;

		static bool isKeyDown(const KeyCode code);
		static void pressKey(const KeyCode code);
		static void releaseKey(const KeyCode code);
		static void toggleKey(const KeyCode code);

		static bool isMouseDown(const MouseButton code);
		static void pressMouseKey(const MouseButton code);
		static void releaseMouseKey(const MouseButton code);
		static void toggleMouseKey(const MouseButton code);

		static void setMousePosition(const double& posX, const double& posY) { i->mousePos = glm::vec2(posX, posY); }
		static glm::vec2 getMousePosition() { return i->mousePos; }

		static bool isMouseScrolling() { return i->mouseScroll.y != 0.f || i->mouseScroll.x != 0.f; }
		static bool isMouseScrollingX() { return i->mouseScroll.x != 0.f; }
		static bool isMouseScrollingY() { return i->mouseScroll.y != 0.f; }
		static void setMouseScroll(const double& posX, const double& posY) { i->mouseScroll = glm::vec2(posX, posY); }
		static glm::vec2 getMouseScroll() { return i->mouseScroll; }
	private:
		static bool initialized;
		glm::vec2 mousePos;
		glm::vec2 mouseScroll;
		static bool keysPressed[];
		static bool mouseBtnsPressed[];
	};

}