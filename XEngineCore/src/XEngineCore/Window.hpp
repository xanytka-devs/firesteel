#pragma once

#include <string>
#include <functional>

#include "XEngineCore/Event.hpp"

struct GLFWwindow;

namespace XEngine {

	class Window {
	public:
		using EventCallback = std::function<void(BaseEvent&)>;

		Window(std::string title, const unsigned int width, const unsigned int height);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator = (const Window&) = delete;
		Window& operator = (Window&&) = delete;

		virtual void update();

		unsigned int getWidth() const { return w_data.width; };
		unsigned int getHeight() const { return w_data.height; };

		void setEventCallback(const EventCallback& callback) {
			w_data.eventCallbackFn = callback;
		}

	private:
		struct WindowData {
			std::string title;
			unsigned int width;
			unsigned int height;
			EventCallback eventCallbackFn;
		};

		int initialize();
		void shutdown();

		GLFWwindow* window = nullptr;
		WindowData w_data;
		float bgColor[4] = { 1.f, 0.f, 0.f, 0.f };

	};

}