#include <firesteel/input/joystick.hpp>

#include <firesteel/common.hpp>

namespace Firesteel {
	Joystick::Joystick() {
		initialize(0);
	}
	Joystick::Joystick(int tID) {
		initialize(tID);
	}

	void Joystick::printInfo() {
		if(!mPresence) {
			LOG_INFO("Joystick: None");
			return;
		}
		LOG_INFO(std::string("Joystick: ") + mName + " (" + std::to_string(mID) + ")");
		LOG_INFO("Axes: " + std::to_string(mAxesCount));
		LOG_INFO("Buttons: " + std::to_string(mButtonCount));
	}

	void Joystick::initialize(const int tID) {
		mID = getGID(tID);
		update();
	}

	void Joystick::update() {
		mPresence = glfwJoystickPresent(mID);
		if (!mPresence) return;
		mName = glfwGetJoystickName(mID);
		mAxes = glfwGetJoystickAxes(mID, &mAxesCount);
		mButtons = glfwGetJoystickButtons(mID, &mButtonCount);
	}

	float Joystick::getAxis(const int tAxis) {
		if(mPresence) return mAxes[tAxis];
		return 0.0f;
	}
	unsigned char Joystick::getButton(const int tButton) {
		if(mPresence) return mButtons[tButton];
		return GLFW_RELEASE;
	}
}