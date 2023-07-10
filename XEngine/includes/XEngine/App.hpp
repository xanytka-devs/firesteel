#pragma once

#include <memory>

#include "XEngine/Event.hpp"
#include "Camera.hpp"

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
		virtual void onUIDraw() { }

		float camPosition[3] = { 0.f, 0.f, 1.f };
		float camRotation[3] = { 0.f, 0.f, 0.f };
		bool camIsPresp = false;
		Camera camera;
		float bgColor[3] = { 0.25f, 0.25f, 0.25f };

	private:
		std::unique_ptr<class Window> mainWindow;

		EventDispatcher eventDispatcher;
		bool closeWindow = false;
	};

}