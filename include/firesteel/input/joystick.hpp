#ifndef FS_JOYSTICK_H
#define FS_JOYSTICK_H

#include <firesteel/common.hpp>

namespace Firesteel {
	enum JoystickControls {
		// From: https://pastebin.com/6rusNyxR
		// Analog input button values.   //      PS      |       XBOX
		BTN_LEFT=0,						//  Square      |   X
		BTN_DOWN=1,						//  X           |   A
		BTN_RIGHT=2,					//  Circle      |   B
		BTN_UP=3,						//  Triangle    |   Y   
		SHOULDER_LEFT=4,				//  L1          |   LB
		SHOULDER_RIGHT=5,				//  R1          |   RB
		TRIGGER_LEFT=6,					//  L2          |   LT
		TRIGGER_RIGHT=7,				//  R2          |   RT
		SELECT=8,						//  Share       |   Address
		START=9,						//  Options     |   Menu
		LEFT_STICK=10,					//  L3          |   LS
		RIGHT_STICK=11,					//  R3          |   RS
		J_HOME=12,						//  Home        |   Home
		CLICK=13,						//  Touch pad   |   n/a
		DPAD_UP=14,						//  Dpad up     |   Dpad up
		DPAD_RIGHT=15,					//  Dpad right  |   Dpad right
		DPAD_DOWN=16,					//  Dpad down   |   Dpad down
		DPAD_LEFT=17,					//  Dpad left   |   Dpad left
		// Axes.
		AXIS_LEFT_STICK_X=0,
		AXIS_LEFT_STICK_Y=1,
		AXIS_RIGHT_STICK_X=2,
		AXIS_LEFT_TRIGGER=3,
		AXIS_RIGHT_TRIGGER=4,
		AXIS_RIGHT_STICK_Y=5,
	};

	class Joystick {
	public:
		// Sets up joystick input.
		Joystick() {
			initialize(0);
		}
		// Sets up joystick input.
		Joystick(int tID) {
			initialize(tID);
		}
		// General info about controller.
		void printInfo() {
			if(!mPresence) {
				LOG_INFO("Joystick: None");
				return;
			}
			LOG_INFO(std::string("Joystick: ") + mName + " (" + std::to_string(mID) + ")");
			LOG_INFO("Axes: " + std::to_string(mAxesCount));
			LOG_INFO("Buttons: " + std::to_string(mButtonCount));
		}
		// Initializes controller.
		void initialize(const int tID) {
			mID=getGID(tID);
			update();
		}

		// Updates variables of joystick.
		void update() {
			mPresence=glfwJoystickPresent(mID);
			if(!mPresence) return;
			mName=glfwGetJoystickName(mID);
			mAxes=glfwGetJoystickAxes(mID, &mAxesCount);
			mButtons=glfwGetJoystickButtons(mID, &mButtonCount);
		}

		float getAxis(const int tAxis) {
			if(mPresence)
				return mAxes[tAxis];
			return 0.0f;
		}
		unsigned char getButton(const int tButton) {
			if(mPresence)
				return mButtons[tButton];
			return GLFW_RELEASE;
		}
		int getAxesCount() const { return mAxesCount; }
		int getButtonCount() const { return mButtonCount; }

		bool isPresent() const { return mPresence; }
		const char* getName() { return mName; }
		int getID() const { return mID; }

		// Get global ID of joystick.
		static int getGID(const int tID) { return GLFW_JOYSTICK_1 + tID; }
	private:
		int mPresence;
		int mID;
		const char* mName;

		int mAxesCount;
		const float* mAxes;

		int mButtonCount;
		const unsigned char* mButtons;
	};
}

#endif // !FS_JOYSTICK_H
