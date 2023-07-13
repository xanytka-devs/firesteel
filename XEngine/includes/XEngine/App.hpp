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
		void exit();
		virtual void update() { }
		virtual void onUIDraw() { }
		virtual void onInitialized() { }

		glm::vec2 getCursorPosition() const;

		float camPosition[3] = { 0.f, 0.f, 0.f };
		float camRotation[3] = { 0.f, 0.f, 0.f };
		bool camIsPresp = true;
		Camera baseCamera{glm::vec3(-5.f, 0.f, 0.f)};

		float bgColor[3] = { 0.5f, 0.5f, 0.5f };
		float position[3] = { 5.f, 5.f, 5.f };
		float rotation[3] = { 0.f, 0.f, 0.f };
		float scale[3] = { 1.f, 1.f, 1.f };
		bool disableAnimations = true;

		float lightSourcePos[3] = { 0.f, 0.f, 1.f };
		float lightSourceColor[3] = { 1.f, 1.f, 1.f };
		float ambientFactor = 0.1f;
		float diffuseFactor = 1.f;
		float specularFactor = 0.5f;
		float shininess = 32.f;
	private:
		void draw();
		std::unique_ptr<class Window> mainWindow;

		EventDispatcher eventDispatcher;
		bool closeWindow = false;
	};

}