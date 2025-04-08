#ifndef FS_INPUT_H
#define FS_INPUT_H

#include <../include/input/keyboard.hpp>
#include <../include/input/mouse.hpp>
#include <../include/input/joystick.hpp>

namespace Firesteel {
	class Input {
	public:
		static float getHorizontalAxis() {
			if(Keyboard::getKey(KeyCode::D)) return 1.f;
			if(Keyboard::getKey(KeyCode::A)) return -1.f;
			return 0.f;
		}
		static float getVerticalAxis() {
			if(Keyboard::getKey(KeyCode::W)) return 1.f;
			if(Keyboard::getKey(KeyCode::S)) return -1.f;
			return 0.f;
		}
	};
}

#endif // !FS_INPUT_H