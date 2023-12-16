#ifndef APP_H
#define APP_H

struct GLFWwindow;

namespace XEngine {

	class App {
	public:
		App();
		virtual ~App();
		static void shutdown();

		static void switch_wireframe(int t_mode);

		App(const App&) = delete;
		App(App&&) = delete;
		App& operator = (const App&) = delete;
		App& operator = (App&&) = delete;

		virtual int start(unsigned int t_win_width, unsigned int t_win_height, const char* t_title);
		virtual void initiate() { }
		virtual void update() { }
	private:
		GLFWwindow* m_window;
		static bool m_quit;
	};

}

#endif // APP_H