#ifndef UI_H
#define UI_H

#include <xengine/app.hpp>
#include <xengine/rendering/material.hpp>

class UI {
public:
	static void setTheme();
	static void draw(XEngine::App* t_app, XEngine::Camera* t_camera, XEngine::Material* t_mat);
	static void update_bg();
	static void update_pos(XEngine::Camera* t_camera);
};

#endif // UI_H