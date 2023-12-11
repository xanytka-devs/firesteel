#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Keyboard {
public:
	static void key_callback(GLFWwindow* t_window, int t_key, int t_scan_code, int t_action, int t_mods);

	static bool key_status(int t_key);
	static bool key_changed(int t_key);
	static bool key_up(int t_key);
	static bool key_down(int t_key);
private:
	static bool m_keys[];
	static bool m_keys_changed[];
};

#endif // KEYBOARD_H
