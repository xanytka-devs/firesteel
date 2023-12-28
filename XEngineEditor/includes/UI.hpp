#ifndef UI_H
#define UI_H

#include <XEngine/App.hpp>

class UI {
public:
	static void setTheme();
	static void draw(XEngine::App* t_app, XEngine::Camera* t_camera);
	static void update_bg();
private:
	static void setupDock(XEngine::App* t_app);
};

#endif // UI_H