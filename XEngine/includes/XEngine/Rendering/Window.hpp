#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace XEngine {
	enum CursorState {
		NONE = 0,
		LOCKED,
		DISABLED,
		HIDDEN
	};

	class Window {
	public:
		Window();

		bool initialize();
		void update();

		static void framebuffer_size_callback(GLFWwindow* t_window, int t_width, int t_height);
		void set_params();
		void close();
		bool closing();

		void set_cursor_state(CursorState t_state);

		static unsigned int width;
		static unsigned int height;
	private:
		GLFWwindow* m_window;
	};
}

#endif // WINDOW_H