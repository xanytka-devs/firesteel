#ifndef APP_H
#define APP_H

struct GLFWwindow;

namespace XEngine {

	class App {
	public:
		App();
		virtual ~App();
		static void shutdown();

		App(const App&) = delete;
		App(App&&) = delete;
		App& operator = (const App&) = delete;
		App& operator = (App&&) = delete;

		virtual int start(unsigned int t_win_width, unsigned int t_win_height, const char* t_title);
		virtual void initiate() { }
		virtual void update() { }
	};

}

#endif // APP_H