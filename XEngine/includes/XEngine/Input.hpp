#pragma once

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
	private:
		static bool keysPressed[];
		static bool mouseBtnsPressed[];
	};

}