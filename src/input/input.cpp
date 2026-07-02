#include <firesteel/input/input.hpp>

namespace Firesteel {
	float Input::getHorizontalAxis() {
		if (Keyboard::getKey(KeyCode::D)
			|| Keyboard::getKey(KeyCode::RIGHT))
			return 1.f;
		if (Keyboard::getKey(KeyCode::A)
			|| Keyboard::getKey(KeyCode::LEFT))
			return -1.f;
		return 0.f;
	}
	float Input::getVerticalAxis() {
		if (Keyboard::getKey(KeyCode::W)
			|| Keyboard::getKey(KeyCode::UP))
			return 1.f;
		if (Keyboard::getKey(KeyCode::S)
			|| Keyboard::getKey(KeyCode::DOWN))
			return -1.f;
		return 0.f;
	}
}