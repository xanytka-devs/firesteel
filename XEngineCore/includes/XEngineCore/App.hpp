#pragma once

#include <memory>

#include "XEngineCore/Event.hpp"

namespace XEngine {

	class App {
	public:
		App();
		virtual ~App();

		App(const App&) = delete;
		App(App&&) = delete;
		App& operator = (const App&) = delete;
		App& operator = (App&&) = delete;

		virtual int start(unsigned int win_width, unsigned int win_height, const char* title);
		virtual void update() { }

	private:
		std::unique_ptr<class Window> mainWindow;

		EventDispatcher eventDispatcher;
		bool closeWindow = false;
	};

}