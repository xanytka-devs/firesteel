#ifndef MOUSE_H
#define MOUSE_H

#include <../../external/glfw/include/GLFW/glfw3.h>

class Mouse {
public:
	/// <summary>
	/// Set mouse cursor position.
	/// </summary>
	/// <param name="t_window">Window handle.</param>
	/// <param name="t_x">Cursor x.</param>
	/// <param name="t_y">Cursor y.</param>
	static void cursor_callback(GLFWwindow* t_window, double t_x, double t_y) {
		m_x = t_x;
		m_y = t_y;
		//Check if first mouse button.
		if(m_first_move) {
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
	static void button_callback(GLFWwindow* t_window, int t_button, int t_action, int t_mods) {
		//Check action.
		if(t_action != GLFW_RELEASE) {
			if(!m_buttons[t_button]) m_buttons[t_button] = true;
		}
		else m_buttons[t_button] = false;
		//Detect if button is pressed continuously.
		m_buttons[t_button] = t_action != GLFW_REPEAT;
	}
	/// <summary>
	/// Set mouse wheel deltas.
	/// </summary>
	/// <param name="t_window">Window handle.</param>
	/// <param name="t_dx">Delta x of scroll.</param>
	/// <param name="t_dy">Delta y of scroll.</param>
	static void scroll_callback(GLFWwindow* t_window, double t_dx, double t_dy) {
		m_wheel_dx = t_dx;
		m_wheel_dy = t_dy;
	}

	/// <summary>
	/// Get cursor x.
	/// </summary>
	/// <returns>X of cursor.</returns>
	static double get_cursor_x() { return m_x; }
	/// <summary>
	/// Get cursor y.
	/// </summary>
	/// <returns>Y of cursor.</returns>
	static double get_cursor_y() { return m_y; }

	/// <summary>
	/// Get cursor x delta.
	/// </summary>
	/// <returns>Delta x of cursor.</returns>
	static double get_cursor_dx() {
		double _dx = m_dx;
		m_dx = 0;
		return _dx;
	}
	/// <summary>
	/// Get cursor y delta.
	/// </summary>
	/// <returns>Delta y of cursor.</returns>
	static double get_cursor_dy() {
		double _dy = m_dy;
		m_dy = 0;
		return _dy;
	}

	/// <summary>
	/// Get mouse wheel x delta.
	/// </summary>
	/// <returns>Delta x of mouse wheel.</returns>
	static double get_wheel_dx() {
		double w_dx = m_wheel_dx;
		m_wheel_dx = 0;
		return w_dx;
	}
	/// <summary>
	/// Get mouse wheel y delta.
	/// </summary>
	/// <returns>Delta y of mouse wheel.</returns>
	static double get_wheel_dy() {
		double w_dy = m_wheel_dy;
		m_wheel_dy = 0;
		return w_dy;
	}

	/// <summary>
	/// Gets current status of mouse button.
	/// </summary>
	/// <param name="t_button">Button index.</param>
	/// <returns>true=pressed;false=not pressed</returns>
	static bool button_state(int t_button) {
		return m_buttons[t_button];
	}
	/// <summary>
	/// Has mouse button changed?
	/// </summary>
	/// <param name="t_button">Button index.</param>
	/// <returns>Last status of button.</returns>
	static bool button_changed(int t_button) {
		bool output = m_buttons[t_button];
		m_buttons[t_button] = false;
		return output;
	}
	/// <summary>
	/// Is mouse button up?
	/// </summary>
	/// <param name="t_button">Button index.</param>
	/// <returns>true=button is pressed;false=button isn't pressed</returns>
	static bool button_up(int t_button) {
		return !m_buttons[t_button] && button_changed(t_button);
	}
	/// <summary>
	/// Is mouse button down?
	/// </summary>
	/// <param name="t_button">Button index.</param>
	/// <returns>true=button is pressed;false=button isn't pressed</returns>
	static bool button_down(int t_button) {
		return m_buttons[t_button] && button_changed(t_button);
	}
private:
	static double m_x;
	static double m_y;
	static double m_old_x;
	static double m_old_y;

	static double m_dx;
	static double m_dy;

	static double m_wheel_dx;
	static double m_wheel_dy;

	static bool m_first_move;
	static bool m_buttons[];
	static bool m_buttons_changed[];
};

#endif // MOUSE_H
