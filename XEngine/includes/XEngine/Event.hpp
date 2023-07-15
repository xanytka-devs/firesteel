#pragma once

#include <functional>
#include <array>

#include "Input/Keys.hpp"

namespace XEngine {

	enum class EventType {
		WindowResize = 0,
		WindowClose,

		KeyDown,
		KeyUp,

		MouseButtonDown,
		MouseButtonUp,
		MouseMove,

		EventsCount
	};
	
	struct BaseEvent {
		virtual ~BaseEvent() = default;
		/// <summary>
		/// What type is this event.
		/// </summary>
		/// <returns>Event type.</returns>
		virtual EventType getType() const = 0;
	};

    class EventDispatcher
    {
    public:
        template<typename EventType>
        void addEventListener(std::function<void(EventType&)> callback) {
            auto baseCallback = [func = std::move(callback)](BaseEvent& e) {
                if (e.getType() == EventType::type) func(static_cast<EventType&>(e));
            };
            eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
        }

        void dispatch(BaseEvent& event) {
            auto& callback = eventCallbacks[static_cast<size_t>(event.getType())];
            if (callback) callback(event);
        }

    private:
        std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> eventCallbacks;
    };

	struct EventMouseMove : public BaseEvent {

		/// <summary>
		/// Main function of event MouseMove.
		/// </summary>
		/// <param name="newX">New mouse X.</param>
		/// <param name="newY">New mouse Y.</param>
		EventMouseMove(const double newX, const double newY)
			: x(newX), y(newY) { }
		virtual EventType getType() const override {
			return type;
		}

		double x, y;
		static const EventType type = EventType::MouseMove;

	};

	struct EventWindowResize : public BaseEvent {

		/// <summary>
		/// Main function of event WindowResize.
		/// </summary>
		/// <param name="newW">New window width.</param>
		/// <param name="newH">New window height.</param>
		EventWindowResize(const unsigned int newW, const unsigned int newH)
			: width(newW), height(newH) { }
		virtual EventType getType() const override {
			return type;
		}

		unsigned int width, height;
		static const EventType type = EventType::WindowResize;

	};

	struct EventWindowClose : public BaseEvent {
		virtual EventType getType() const override{
			return type;
		}

		static const EventType type = EventType::WindowClose;
	};
	
	struct EventKeyDown : public BaseEvent {
		EventKeyDown(const KeyCode key_code, const bool repeated)
			: keyCode(key_code), repeated(repeated) {
		}

		virtual EventType getType() const override {
			return type;
		}

		KeyCode keyCode;
		bool repeated;

		static const EventType type = EventType::KeyDown;
	};

	struct EventKeyUp : public BaseEvent{
		EventKeyUp(const KeyCode key_code)
			: keyCode(key_code) {
		}

		virtual EventType getType() const override {
			return type;
		}

		KeyCode keyCode;

		static const EventType type = EventType::KeyUp;
	};

	struct EventMouseButtonDown : public BaseEvent {
		EventMouseButtonDown(const MouseButton mouse_button, const double xPos, const double yPos)
			: mouseButton(mouse_button)
			, x(xPos)
			, y(yPos) {
		}

		virtual EventType getType() const override {
			return type;
		}

		MouseButton mouseButton;
		double x;
		double y;

		static const EventType type = EventType::MouseButtonDown;
	};

	struct EventMouseButtonUp : public BaseEvent {
		EventMouseButtonUp(const MouseButton mouse_button, const double xPos, const double yPos)
			: mouseButton(mouse_button)
			, x(xPos)
			, y(yPos) {
		}

		virtual EventType getType() const override {
			return type;
		}

		MouseButton mouseButton;
		double x;
		double y;

		static const EventType type = EventType::MouseButtonUp;
	};
	
}