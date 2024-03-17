#include "xengine/input/keyboard.hpp"

bool Keyboard::m_keys[GLFW_KEY_LAST] = { 0 };
bool Keyboard::m_keys_changed[GLFW_KEY_LAST] = { 0 };

#include "xengine/input/mouse.hpp"

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