#pragma once

namespace XEngine {

	class App {
	public:
		App();
		virtual ~App();

		App(const App&) = delete;
		App(App&&) = delete;
		App& operator = (const App&) = delete;
		App& operator = (App&&) = delete;

		virtual int start(unsigned int t_win_width, unsigned int t_win_height, const char* t_title);
		virtual void update() { }
	};

}