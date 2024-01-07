#ifndef UI_H
#define UI_H

#include <XEngine/App.hpp>

class UI {
public:
	static void setTheme();
	static void draw(XEngine::App* t_app, XEngine::Camera* t_camera);
	static void update_bg();
	static void update_pos(XEngine::Camera* t_camera);
};

#endif // UI_H