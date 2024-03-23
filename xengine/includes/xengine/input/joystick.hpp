#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <../../external/glfw/include/GLFW/glfw3.h>

enum JoystickControls {
	//From: https://pastebin.com/6rusNyxR
	//Analog input button values.   //      PS      |       XBOX
	BTN_LEFT			= 0,        //  Square      |   X
	BTN_DOWN			= 1,        //  X           |   A
	BTN_RIGHT			= 2,        //  Circle      |   B
	BTN_UP				= 3,        //  Triangle    |   Y   
	SHOULDER_LEFT		= 4,		//  L1          |   LB
	SHOULDER_RIGHT		= 5,		//  R1          |   RB
	TRIGGER_LEFT		= 6,		//  L2          |   LT
	TRIGGER_RIGHT		= 7,		//  R2          |   RT
	SELECT				= 8,        //  Share       |   Address
	START				= 9,        //  Options     |   Menu
	LEFT_STICK			= 10,		//  L3          |   LS
	RIGHT_STICK			= 11,		//  R3          |   RS
	J_HOME				= 12,       //  Home        |   Home
	CLICK				= 13,       //  Touch pad   |   n/a
	DPAD_UP				= 14,		//  Dpad up     |   Dpad up
	DPAD_RIGHT			= 15,		//  Dpad right  |   Dpad right
	DPAD_DOWN			= 16,       //  Dpad down   |   Dpad down
	DPAD_LEFT			= 17,       //  Dpad left   |   Dpad left
	//Axes.
	AXES_LEFT_STICK_X	= 0,
	AXES_LEFT_STICK_Y	= 1,
	AXES_RIGHT_STICK_X	= 2,
	AXES_LEFT_TRIGGER	= 3,
	AXES_RIGHT_TRIGGER	= 4,
	AXES_RIGHT_STICK_Y	= 5,
};

class Joystick {
public:
	/// <summary>
	/// Sets up joystick input.
	/// </summary>
	Joystick() {
		m_id = get_id(0);
		update();
	}
	/// <summary>
	/// Sets up joystick input.
	/// </summary>
	/// <param name="t_id">ID for joystick.</param>
	Joystick(int t_id) {
		m_id = get_id(t_id);
		update();
	}
	/// <summary>
	/// Updates variables of joystick.
	/// </summary>
	void update() {
		m_presence = glfwJoystickPresent(m_id);
		if(!m_presence) return;
		m_name = glfwGetJoystickName(m_id);
		m_axes = glfwGetJoystickAxes(m_id, &m_axes_count);
		m_buttons = glfwGetJoystickButtons(m_id, &m_button_count);
	}

	/// <summary>
	/// Get state of axis.
	/// </summary>
	/// <param name="t_axis">Axis ID.</param>
	/// <returns>State in float.</returns>
	float axis_state(int t_axis) {
		if(m_presence)
			return m_axes[t_axis];
		return 0.0f;
	}
	/// <summary>
	/// Get state of button.
	/// </summary>
	/// <param name="t_button">Button ID.</param>
	/// <returns>State of button.</returns>
	unsigned char button_state(int t_button) {
		if(m_presence)
			return m_buttons[t_button];
		return GLFW_RELEASE;
	}

	/// <summary>
	/// Get axes count.
	/// </summary>
	/// <returns>Amount of axes.</returns>
	int get_axes_count() const {
		return m_axes_count;
	}
	/// <summary>
	/// Get buttons count.
	/// </summary>
	/// <returns>Amount of buttons.</returns>
	int get_button_count() const {
		return m_button_count;
	}

	/// <summary>
	/// Get current joystick state.
	/// </summary>
	/// <returns>true=present;false=isn't present</returns>
	bool is_present() const {
		return m_presence;
	}
	/// <summary>
	/// Get joystick name.
	/// </summary>
	/// <returns>Name of joystick.</returns>
	const char* get_name() {
		return m_name;
	}
	/// <summary>
	/// Get input ID of joystick.
	/// </summary>
	/// <param name="t_i">Base ID of joystick.</param>
	/// <returns>Input ID of joystick.</returns>
	static int get_id(int t_i) {
		return GLFW_JOYSTICK_1 + t_i;
	}
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
