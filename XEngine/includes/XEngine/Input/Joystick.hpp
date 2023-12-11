#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//From: https://pastebin.com/6rusNyxR
//Analog input button values.                   //      PS      |       XBOX
#define GLFW_JOYSTICK_BTN_LEFT 0                //  Square      |   X
#define GLFW_JOYSTICK_BTN_DOWN 1                //  X           |   A
#define GLFW_JOYSTICK_BTN_RIGHT 2               //  Circle      |   B
#define GLFW_JOYSTICK_BTN_UP 3                  //  Triangle    |   Y   
#define GLFW_JOYSTICK_SHOULDER_LEFT 4           //  L1          |   LB
#define GLFW_JOYSTICK_SHOULDER_RIGHT 5          //  R1          |   RB
#define GLFW_JOYSTICK_TRIGGER_LEFT 6            //  L2          |   LT
#define GLFW_JOYSTICK_TRIGGER_RIGHT 7           //  R2          |   RT
#define GLFW_JOYSTICK_SELECT 8                  //  Share       |   Address
#define GLFW_JOYSTICK_START 9                   //  Options     |   Menu
#define GLFW_JOYSTICK_LEFT_STICK 10             //  L3          |   LS
#define GLFW_JOYSTICK_RIGHT_STICK 11            //  R3          |   RS
#define GLFW_JOYSTICK_HOME 12                   //  Home        |   Home
#define GLFW_JOYSTICK_CLICK 13                  //  Touch pad   |   n/a
#define GLFW_JOYSTICK_DPAD_UP 14                //  Dpad up     |   Dpad up
#define GLFW_JOYSTICK_DPAD_RIGHT 15             //  Dpad right  |   Dpad right
#define GLFW_JOYSTICK_DPAD_DOWN 16              //  Dpad down   |   Dpad down
#define GLFW_JOYSTICK_DPAD_LEFT 17              //  Dpad left   |   Dpad left
//Axes.
#define GLFW_JOYSTICK_AXES_LEFT_STICK_X 0
#define GLFW_JOYSTICK_AXES_LEFT_STICK_Y 1
#define GLFW_JOYSTICK_AXES_RIGHT_STICK_X 2
#define GLFW_JOYSTICK_AXES_LEFT_TRIGGER 3
#define GLFW_JOYSTICK_AXES_RIGHT_TRIGGER 4
#define GLFW_JOYSTICK_AXES_RIGHT_STICK_Y 5

class Joystick {
public:
	Joystick(int t_id);
	void update();

	float axes_state(int t_axis);
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
