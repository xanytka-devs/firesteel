#include "XEngine/Input/Joystick.hpp"

Joystick::Joystick(int t_id);
void update();

float axes_state(int t_axis);
unsigned char button_state(int t_button);

int get_axes_count();
int get_button_count();

bool is_present();
const char* get_name();
static int get_id(int t_i);