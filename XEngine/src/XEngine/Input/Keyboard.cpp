#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "XEngine/Input/Keyboard.hpp"

bool Keyboard::m_keys[GLFW_KEY_LAST] = { 0 };
bool Keyboard::m_keys_changed[GLFW_KEY_LAST] = { 0 };

/// <summary>
/// Happens on every input callback.
/// </summary>
/// <param name="t_window">Window handle.</param>
/// <param name="t_key">Key index.</param>
/// <param name="t_scan_code">Scan code.</param>
/// <param name="t_action">Curent action.</param>
/// <param name="t_mods">Modifier.</param>
void Keyboard::key_callback(GLFWwindow* t_window, int t_key, int t_scan_code, int t_action, int t_mods) {
	//Check action.
	if(t_action != GLFW_RELEASE) {
		if(!m_keys[t_key]) m_keys[t_key] = true;
	} else m_keys[t_key] = false;
	//Detect if key is pressed continuously.
	m_keys_changed[t_key] = t_action != GLFW_REPEAT;
}

/// <summary>
/// Gets current status of key.
/// </summary>
/// <param name="t_key">Key index.</param>
/// <returns>true=pressed;false=not pressed</returns>
bool Keyboard::key_state(int t_key) {
	return m_keys[t_key];
}

/// <summary>
/// Has key changed?
/// </summary>
/// <param name="t_key">Key index.</param>
/// <returns>Last status of key.</returns>
bool Keyboard::key_changed(int t_key) {
	bool output = m_keys_changed[t_key];
	m_keys_changed[t_key] = false;
	return output;
}

/// <summary>
/// Is key up?
/// </summary>
/// <param name="t_key">Key index.</param>
/// <returns>true=key isn't pressed;false=key is pressed</returns>
bool Keyboard::key_up(int t_key) {
	return !m_keys[t_key] && key_changed(t_key);
}

/// <summary>
/// Is key down?
/// </summary>
/// <param name="t_key">Key index.</param>
/// <returns>true=key is pressed;false=key isn't pressed</returns>
bool Keyboard::key_down(int t_key) {
	return m_keys[t_key] && key_changed(t_key);
}