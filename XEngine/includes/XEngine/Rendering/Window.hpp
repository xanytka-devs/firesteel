#ifndef WINDOW_H
#define WINDOW_H

struct GLFWwindow;

namespace XEngine {
	enum CursorState {
		NONE = 0,
		LOCKED,
		DISABLED,
		HIDDEN
	};

	//Delete to remove 60 fps cap. Disables double buffer.
#define RENDER_CAP_60_NDB
	//Delete to remove 60 fps cap. Changes swap delay to 0.
#define RENDER_CAP_60_SWAP

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

		void disable_cap_60();
	private:
		GLFWwindow* m_window;
		bool cap_60 = true;
	};
}

#endif // WINDOW_H