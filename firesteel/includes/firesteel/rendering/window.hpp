#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#ifdef FS_IMGUI
//Includes ImGui without need to add it to CMake project.
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#endif // FS_IMGUI
#include "firesteel/math.hpp"
#include "firesteel/common.hpp"

struct GLFWwindow;
struct ImFont;

struct GuiFont {
	ImFont* font;
	const char* path;
	float size;
};

namespace firesteel {

	enum WindowParam {
		W_NONE = 0x0, //No property.
		W_TITLE = 0x1, //Window title.
		W_CURSOR = 0x2, //State of cursor.
		W_VSYNC = 0x3, //Use Vertical Sync?
		W_OPACITY = 0x4, //Window transparency.
		W_POS = 0x5, //Position of window.
		W_SIZE = 0x6, //Size of window.
		W_ICON = 0x7, //Icon for window.
		W_TITLE_BAR = 0x8 //Decorate the window's titlebar?
	};

	enum CursorState {
		C_NONE = 0x0, //No property.
		C_LOCKED = 0x1, //Cursor is locked into window's borders.
		C_DISABLED = 0x2, //Cursor is locked and hidden.
		C_HIDDEN = 0x3 //Cursor is just hidden.
	};

	class Window {
	public:
		Window();
		Window(unsigned int t_width, unsigned int t_height, const char* t_title);
		Window(unsigned int t_width, unsigned int t_height, std::string t_title);

		/// <summary>
		/// Window initialization.
		/// </summary>
		/// <returns>true=success;false=failure</returns>
		bool initialize();
		/// <summary>
		/// Updates events and clears buffers.
		/// </summary>
		void update();
		void pull_events();
		/// <summary>
		/// Closes window.
		/// </summary>
		void close();

		/// <summary>
		/// Initializes ImGui.
		/// </summary>
		void gui_initialize();
		/// <summary>
		/// [HANDELED BY APP CLASS]
		/// Updates ImGui.
		/// </summary>
		void gui_update();
		/// <summary>
		/// [HANDELED BY APP CLASS]
		/// Draws ImGui.
		/// </summary>
		void gui_draw();
		/// <summary>
		/// Shuts down ImGui.
		/// </summary>
		void gui_shutdown();

		void set_param(WindowParam t_param, bool t_val);
		void set_param(WindowParam t_param, int t_val);
		void set_param(WindowParam t_param, glm::vec2 t_vec);
		void set_param(WindowParam t_param, const char* t_val);
		void set_param(WindowParam t_param, std::string t_val);
		/// <summary>
		/// Sets up all parameters.
		/// </summary>
		void set_init_params();
		void set_drop_callback(void (*t_drop_callback)(int count, const char** paths));

		int get_param_i(WindowParam t_param) const;
		bool get_param_b(WindowParam t_param) const;
		glm::vec2 size() { return glm::vec2(width, height); }
		/// <summary>
		/// Checks if window's going to close.
		/// </summary>
		/// <returns></returns>
		bool closing();
		GLFWwindow* get_handle() { return m_window; }

		static unsigned int width;
		static unsigned int height;

		std::vector<GuiFont> ui_fonts;
		bool ui_need_to_reload = false;
	private:
		static void framebuffer_callback(GLFWwindow* t_window, int t_width, int t_height);
		static void m_drop_callback(GLFWwindow* t_window, int t_count, const char** t_paths);

		GLFWwindow* m_window;
		CursorState m_cur_state = CursorState::C_NONE;
		std::string m_title;
		bool m_vsync = false;
	};
}

#endif // WINDOW_H