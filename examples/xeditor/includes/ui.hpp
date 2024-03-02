#ifndef UI_H
#define UI_H

#include <xengine/enviroment.hpp>
#include <xengine/app.hpp>
#include <xengine/rendering/material.hpp>

class UI {
public:
	struct UIEditorData {
		XEngine::App* t_app;
		XEngine::Camera* t_camera;
		Cube* t_cube;
		Transform* t_model;
		Scene* t_scene;
	};

	static void init();
	static void setTheme();
	static void draw(UIEditorData t_data);
	static void update_pos(XEngine::Camera* t_camera);
};

#endif // UI_H