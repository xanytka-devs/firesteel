#ifndef WINDOW_H
#define WINDOW_H

#include <string>
struct GLFWwindow;

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
		Window(unsigned int t_width, unsigned int t_height, const char* t_title);
		Window(unsigned int t_width, unsigned int t_height, std::string t_title);

		bool initialize();
		void update();
		void ui_initialize();
		void ui_update();
		void ui_draw();
		void ui_shutdown();

		static void framebuffer_size_callback(GLFWwindow* t_window, int t_width, int t_height);
		void set_params();
		void close();
		bool closing();

		void set_cursor_state(CursorState t_state);

		void set_title(const char* t_title);
		static unsigned int width;
		static unsigned int height;
	private:
		GLFWwindow* m_window;
		std::string m_title;
	};
}

#endif // WINDOW_H