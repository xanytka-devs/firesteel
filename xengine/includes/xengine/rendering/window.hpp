#ifndef WINDOW_H
#define WINDOW_H

#include <string>
struct GLFWwindow;

namespace XEngine {

	enum WindowParam {
		W_NONE = 0,
		W_TITLE,
		W_CURSOR,
		W_VSYNC
	};

	enum CursorState {
		C_NONE = 0,
		C_LOCKED,
		C_DISABLED,
		C_HIDDEN
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
		void set_param(WindowParam t_param, bool t_val);
		void set_param(WindowParam t_param, int t_val);
		void set_param(WindowParam t_param, const char* t_val);
		void set_param(WindowParam t_param, std::string t_val);
		void set_init_params();
		void close();

		int get_param_i(WindowParam t_param);
		bool get_param_b(WindowParam t_param);
		bool closing();

		static unsigned int width;
		static unsigned int height;
	private:
		GLFWwindow* m_window;
		CursorState m_cur_state;
		std::string m_title;
		bool m_vsync = false;
	};
}

#endif // WINDOW_H