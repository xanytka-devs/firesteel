#include <iostream>

#include "XEngineCore/App.hpp"
#include "XEngineCore/Log.hpp"
#include "XEngineCore/Window.hpp"
#include "XEngineCore/Event.hpp"

namespace XEngine {
	
	/// <summary>
	/// Occures at app startup (instantiation).
	/// </summary>
	App::App() {
        LOG_INFO("Initializing XEngine Core App.");
    }

	/// <summary>
	/// Occures on app shutdown (destruction).
	/// </summary>
	App::~App() {
		LOG_INFO("Shuting down XEngine Core App.");
	}

	/// <summary>
	/// Creates new instance of window for application.
	/// </summary>
	/// <param name="win_width">Width of the window.</param>
	/// <param name="win_height">Height of the window.</param>
	/// <param name="title">Name of the window.</param>
	/// <returns>Exit code. Only 0 is success.</returns>
	int App::start(unsigned int win_width, unsigned int win_height, const char* title) {

		//Create window pointer.
		mainWindow = std::make_unique<Window>(title, win_width, win_height);
		//Add event listeners.
		eventDispatcher.addEventListener<EventWindowClose>([&](EventWindowClose& event) {
			closeWindow = true;
		});
		//Apply them.
		mainWindow->setEventCallback([&](BaseEvent& e) {
			eventDispatcher.dispatch(e);
		});

		while (!closeWindow) {
			mainWindow->update();
			update();
		}
		mainWindow = nullptr;

        return 0;

	}

}