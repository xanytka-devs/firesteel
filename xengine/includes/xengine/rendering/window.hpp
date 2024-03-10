#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include "../../external/imgui/imgui.h"
struct GLFWwindow;

namespace XEngine {

	enum WindowParam {
		W_NONE = 0x0,
		W_TITLE = 0x1,
		W_CURSOR = 0x2,
		W_VSYNC = 0x3,
		W_OPACITY = 0x4,
		W_POS = 0x5,
		W_SIZE = 0x6
	};

	enum CursorState {
		C_NONE = 0x0,
		C_LOCKED = 0x1,
		C_DISABLED = 0x2,
		C_HIDDEN = 0x3
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

		void set_param(WindowParam t_param, bool t_val);
		void set_param(WindowParam t_param, float t_val1, float t_val2 = 0.f);
		void set_param(WindowParam t_param, const char* t_val);
		void set_param(WindowParam t_param, std::string t_val);
		void set_init_params();
		void close();

		int get_param_i(WindowParam t_param) const;
		bool get_param_b(WindowParam t_param) const;
		bool closing();

		static unsigned int width;
		static unsigned int height;
	private:
		static void framebuffer_callback(GLFWwindow* t_window, int t_width, int t_height);
		static void size_callback(GLFWwindow* t_window, int t_width, int t_height);

		GLFWwindow* m_window;
		CursorState m_cur_state = CursorState::C_NONE;
		std::string m_title;
		bool m_vsync = false;
	};
}

#endif // WINDOW_H