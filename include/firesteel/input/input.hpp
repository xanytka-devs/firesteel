#ifndef FS_INPUT_H
#define FS_INPUT_H

#include <firesteel/input/keyboard.hpp>
#include <firesteel/input/mouse.hpp>
#include <firesteel/input/joystick.hpp>

namespace Firesteel {
	class Input {
	public:
		static float getHorizontalAxis();
		static float getVerticalAxis();
	};
}

#endif // !FS_INPUT_H