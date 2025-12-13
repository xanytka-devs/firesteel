#ifndef FS_INPUT_H
#define FS_INPUT_H

#include <firesteel/input/keyboard.hpp>
#include <firesteel/input/mouse.hpp>
#include <firesteel/input/joystick.hpp>

namespace Firesteel {
	class Input {
	public:
		static float getHorizontalAxis() {
			if(Keyboard::getKey(KeyCode::D)
				||Keyboard::getKey(KeyCode::RIGHT))
				return 1.f;
			if(Keyboard::getKey(KeyCode::A)
				||Keyboard::getKey(KeyCode::LEFT))
				return -1.f;
			return 0.f;
		}
		static float getVerticalAxis() {
			if(Keyboard::getKey(KeyCode::W)
				||Keyboard::getKey(KeyCode::UP))
				return 1.f;
			if(Keyboard::getKey(KeyCode::S)
				||Keyboard::getKey(KeyCode::DOWN))
				return -1.f;
			return 0.f;
		}
	};
}

#endif // !FS_INPUT_H