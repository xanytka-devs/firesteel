#pragma once

struct GLFWwindow;

namespace XEngine::UI {

	class TUI {
	public:
		static void init(GLFWwindow* window);
		static void shutdown();
		static void update();
		static void draw();
	};

}