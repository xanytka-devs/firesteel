#ifndef APP_H
#define APP_H

#include "rendering/window.hpp"
#include "enviroment.hpp"

namespace XEngine {

	class App {
	public:
		static App* current_app;

		App();
		virtual ~App();
		void shutdown();
		void update_loop_call();

		App(const App&) = delete;
		App(App&&) = delete;
		App& operator = (const App&) = delete;
		App& operator = (App&&) = delete;

		virtual int start(unsigned int t_win_width, unsigned int t_win_height, const char* t_title);
		virtual void preinitiazile() { }
		virtual void initiazile() { }
		virtual void update() { }
		virtual void on_shutdown() { }

		Window window;
		int fps = 0;
	};

}

#endif // APP_H