#ifndef MOUSE_H
#define MOUSE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mouse {
public:
	static void cursor_callback(GLFWwindow* t_window, double t_x, double t_y);
	static void button_callback(GLFWwindow* t_window, int t_button, int t_action, int t_mods);
	static void scroll_callback(GLFWwindow* t_window, double t_dx, double t_dy);

	static double get_cursor_x();
	static double get_cursor_y();

	static double get_cursor_dx();
	static double get_cursor_dy();

	static double get_wheel_dx();
	static double get_wheel_dy();

	static bool key_status(int t_button);
	static bool key_changed(int t_button);
	static bool key_up(int t_button);
	static bool key_down(int t_button);
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
