#include "XEngine/Input/Joystick.hpp"

/// <summary>
/// Sets up joystick input.
/// </summary>
/// <param name="t_id">ID for joystick.</param>
Joystick::Joystick(int t_id) {
	m_id = get_id(t_id);
	update();
}

/// <summary>
/// Updates variables of joystick.
/// </summary>
void Joystick::update() {
	m_presence = glfwJoystickPresent(m_id);
	if(m_presence) {
		m_name = glfwGetJoystickName(m_id);
		m_axes = glfwGetJoystickAxes(m_id, &m_axes_count);
		m_buttons = glfwGetJoystickButtons(m_id, &m_button_count);
	}
}

/// <summary>
/// Get state of axis.
/// </summary>
/// <param name="t_axis">Axis ID.</param>
/// <returns>State in float.</returns>
float Joystick::axis_state(int t_axis) {
	if(m_presence) {
		return m_axes[t_axis];
	}
	return 0.0f;
}

/// <summary>
/// Get state of button.
/// </summary>
/// <param name="t_button">Button ID.</param>
/// <returns>State of button.</returns>
unsigned char Joystick::button_state(int t_button) {
	if (m_presence) {
		return m_buttons[t_button];
	}
	return GLFW_RELEASE;
}

/// <summary>
/// Get axes count.
/// </summary>
/// <returns>Amount of axes.</returns>
int Joystick::get_axes_count() {
	return m_axes_count;
}

/// <summary>
/// Get buttons count.
/// </summary>
/// <returns>Amount of buttons.</returns>
int Joystick::get_button_count() {
	return m_button_count;
}

/// <summary>
/// Get current joystick state.
/// </summary>
/// <returns>true=present;false=isn't present</returns>
bool Joystick::is_present() {
	return m_presence;
}

/// <summary>
/// Get joystick name.
/// </summary>
/// <returns>Name of joystick.</returns>
const char* Joystick::get_name() {
	return m_name;
}

/// <summary>
/// Get input ID of joystick.
/// </summary>
/// <param name="t_i">Base ID of joystick.</param>
/// <returns>Input ID of joystick.</returns>
int Joystick::get_id(int t_i) {
	return GLFW_JOYSTICK_1 + t_i;
}