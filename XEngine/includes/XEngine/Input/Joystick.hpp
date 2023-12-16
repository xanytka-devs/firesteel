#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum JoystickControls {
	//From: https://pastebin.com/6rusNyxR
	//Analog input button values.   //      PS      |       XBOX
	BTN_LEFT			=0,         //  Square      |   X
	BTN_DOWN			=1,         //  X           |   A
	BTN_RIGHT			=2,         //  Circle      |   B
	BTN_UP				=3,         //  Triangle    |   Y   
	SHOULDER_LEFT		=4,			//  L1          |   LB
	SHOULDER_RIGHT		=5,			//  R1          |   RB
	TRIGGER_LEFT		=6,			//  L2          |   LT
	TRIGGER_RIGHT		=7,			//  R2          |   RT
	SELECT				=8,         //  Share       |   Address
	START				=9,         //  Options     |   Menu
	LEFT_STICK			=10,		//  L3          |   LS
	RIGHT_STICK			=11,		//  R3          |   RS
	J_HOME				=12,        //  Home        |   Home
	CLICK				=13,        //  Touch pad   |   n/a
	DPAD_UP				=14,		//  Dpad up     |   Dpad up
	DPAD_RIGHT			=15,		//  Dpad right  |   Dpad right
	DPAD_DOWN			=16,        //  Dpad down   |   Dpad down
	DPAD_LEFT			=17,        //  Dpad left   |   Dpad left
	//Axes.
	AXES_LEFT_STICK_X	=0,
	AXES_LEFT_STICK_Y	=1,
	AXES_RIGHT_STICK_X	=2,
	AXES_LEFT_TRIGGER	=3,
	AXES_RIGHT_TRIGGER	=4,
	AXES_RIGHT_STICK_Y	=5,
};

class Joystick {
public:
	Joystick(int t_id);
	void update();

	float axis_state(int t_axis);
	unsigned char button_state(int t_button);

	int get_axes_count();
	int get_button_count();

	bool is_present();
	const char* get_name();
	static int get_id(int t_i);
private:
	int m_presence;
	int m_id;
	const char* m_name;

	int m_axes_count;
	const float* m_axes;

	int m_button_count;
	const unsigned char* m_buttons;
};

#endif // JOYSTICK_H
