#ifndef APP_H
#define APP_H

#include "XEngine/Rendering/Window.hpp"

namespace XEngine {

	class App {
	public:
		App();
		virtual ~App();
		void shutdown();

		App(const App&) = delete;
		App(App&&) = delete;
		App& operator = (const App&) = delete;
		App& operator = (App&&) = delete;

		virtual int start(unsigned int t_win_width, unsigned int t_win_height, const char* t_title);
		virtual void initiazile() { }
		virtual void update() { }
		virtual void on_shutdown() { }

		Window window;
		float delta_time = 0.f;
	};

}

#endif // APP_H