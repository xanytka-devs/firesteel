#pragma once

#include <XEngine/App.hpp>

class UI {
public:
	static void setTheme();
	static void draw(XEngine::App* app);
private:
	static void setupDock(XEngine::App* app);
};