#include "XEngine/Input/Mouse.hpp"

double Mouse::m_x = 0;
double Mouse::m_y = 0;
double Mouse::m_old_x = 0;
double Mouse::m_old_y = 0;

double Mouse::m_dx = 0;
double Mouse::m_dy = 0;

double Mouse::m_wheel_dx = 0;
double Mouse::m_wheel_dy = 0;

bool Mouse::m_first_move = true;
bool Mouse::m_buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Mouse::m_buttons_changed[GLFW_MOUSE_BUTTON_LAST] = { 0 };

void Mouse::cursor_callback(GLFWwindow* t_window, double t_x, double t_y) {
	m_x = t_x;
	m_y = t_y;
	//Check if first mouse button.
	if (m_first_move) {
		m_old_x = t_x;
		m_old_y = t_y;
		m_first_move = false;
	}
	//Set deltas.
	m_dx = m_x - m_old_x;
	m_dy = m_old_y - m_y;
	//Set old positions
	m_old_x = m_x;
	m_old_y = m_y;
}

/// <summary>
/// Set mouse buttons statuses.
/// </summary>
/// <param name="t_window">Window handle.</param>
/// <param name="t_button">Button index.</param>
/// <param name="t_action">Curent action.</param>
/// <param name="t_mods">Modifiers.</param>
void Mouse::button_callback(GLFWwindow* t_window, int t_button, int t_action, int t_mods) {
	//Check action.
	if (t_action != GLFW_RELEASE) {
		if (!m_buttons[t_button]) m_buttons[t_button] = true;
	} else m_buttons[t_button] = false;
	//Detect if button is pressed continuously.
	m_buttons[t_button] = t_action != GLFW_REPEAT;
}

/// <summary>
/// Set mouse wheel deltas.
/// </summary>
/// <param name="t_window">Window handle.</param>
/// <param name="t_dx">Delta x of scroll.</param>
/// <param name="t_dy">Delta y of scroll.</param>
void Mouse::scroll_callback(GLFWwindow* t_window, double t_dx, double t_dy) {
	m_wheel_dx = t_dx;
	m_wheel_dy = t_dy;
}

/// <summary>
/// Get cursor x.
/// </summary>
/// <returns>X of cursor.</returns>
double Mouse::get_cursor_x() {
	return m_x;
}

/// <summary>
/// Get cursor y.
/// </summary>
/// <returns>Y of cursor.</returns>
double Mouse::get_cursor_y() {
	return m_y;
}

/// <summary>
/// Get cursor x delta.
/// </summary>
/// <returns>Delta x of cursor.</returns>
double Mouse::get_cursor_dx() {
	return m_dx;
}

/// <summary>
/// Get cursor y delta.
/// </summary>
/// <returns>Delta y of cursor.</returns>
double Mouse::get_cursor_dy() {
	return m_dy;
}

/// <summary>
/// Get mouse wheel x delta.
/// </summary>
/// <returns>Delta x of mouse wheel.</returns>
double Mouse::get_wheel_dx() {
	return m_wheel_dx;
}

/// <summary>
/// Get mouse wheel y delta.
/// </summary>
/// <returns>Delta y of mouse wheel.</returns>
double Mouse::get_wheel_dy() {
	return m_wheel_dy;
}

/// <summary>
/// Gets current status of mouse button.
/// </summary>
/// <param name="t_button">Button index.</param>
/// <returns>true=pressed;false=not pressed</returns>
bool Mouse::key_status(int t_button) {
	return m_buttons[t_button];
}

/// <summary>
/// Has mouse button changed?
/// </summary>
/// <param name="t_button">Button index.</param>
/// <returns>Last status of button.</returns>
bool Mouse::key_changed(int t_button) {
	bool output = m_buttons[t_button];
	m_buttons[t_button] = false;
	return output;
}

/// <summary>
/// Is mouse button up?
/// </summary>
/// <param name="t_button">Button index.</param>
/// <returns>true=button is pressed;false=button isn't pressed</returns>
bool Mouse::key_up(int t_button) {
	return !m_buttons[t_button] && key_changed(t_button);
}

/// <summary>
/// Is mouse button down?
/// </summary>
/// <param name="t_button">Button index.</param>
/// <returns>true=button is pressed;false=button isn't pressed</returns>
bool Mouse::key_down(int t_button) {
	return m_buttons[t_button] && key_changed(t_button);
}