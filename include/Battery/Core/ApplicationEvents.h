#pragma once

#include "Battery/pch.h"
#include "Battery/Log/Log.h"
#include "Battery/Core/Event.h"
#include "Battery/StringUtils.h"

#undef CreateEvent

union ALLEGRO_EVENT;

namespace Battery {

	class WindowCloseEvent : public Battery::Event {
	public:
		WindowCloseEvent() {}

		void Load(ALLEGRO_EVENT* event);

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowClose);
		EVENT_INFO_STRING("");
	};

	class WindowResizeEvent : public Battery::Event {
	public:
		WindowResizeEvent() {}

		void Load(ALLEGRO_EVENT* event);

		int width = 0;
		int height = 0;
		glm::ivec2 windowPosition;

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowResize);
		EVENT_INFO_STRING(" width=%d height=%d windowPosition: (%d, %d)", 
			width, height, windowPosition.x, windowPosition.y
		);
	};

	class WindowFocusEvent : public Battery::Event {
	public:
		WindowFocusEvent() {}

		void Load(ALLEGRO_EVENT* event);

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowFocus);
		EVENT_INFO_STRING("");
	};

	class WindowLostFocusEvent : public Battery::Event {
	public:
		WindowLostFocusEvent() {}

		void Load(ALLEGRO_EVENT* event);

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowLostFocus);
		EVENT_INFO_STRING("");
	};

	class WindowMouseEnteredEvent : public Battery::Event {
	public:
		WindowMouseEnteredEvent() {}

		void Load(ALLEGRO_EVENT* event);

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowMouseEntered);
		EVENT_INFO_STRING("");
	};

	class WindowMouseLeftEvent : public Battery::Event {
	public:
		WindowMouseLeftEvent() {}

		void Load(ALLEGRO_EVENT* event);

		// These are for debugging only
		EVENT_CLASS_TYPE(WindowMouseLeft);
		EVENT_INFO_STRING("");
	};

	class KeyPressedEvent : public Battery::Event {
	public:
		KeyPressedEvent() {}

		void Load(ALLEGRO_EVENT* event);

		int keycode;
		unsigned int modifiers;
		ALLEGRO_DISPLAY* display;
		bool repeat;

		// These are for debugging only
		EVENT_CLASS_TYPE(KeyPressed);
		EVENT_INFO_STRING("KeyCode: %d Modifiers: %d Allegro display: %p Repeat: %s", 
			keycode, modifiers, display, repeat ? "true" : "false");
	};

	class KeyReleasedEvent : public Battery::Event {
	public:
		KeyReleasedEvent() {}

		void Load(ALLEGRO_EVENT* event);

		int keycode;
		unsigned int modifiers;
		ALLEGRO_DISPLAY* display;
		bool repeat;

		// These are for debugging only
		EVENT_CLASS_TYPE(KeyReleased);
		EVENT_INFO_STRING("KeyCode: %d Modifiers: %d Allegro display: %p Repeat: %s",
			keycode, modifiers, display, repeat ? "true" : "false");
	};

	class TextInputEvent : public Battery::Event {
	public:
		TextInputEvent() {}

		void Load(ALLEGRO_EVENT* event);

		int32_t codepoint;
		char character[5];
		unsigned int modifiers;
		ALLEGRO_DISPLAY* display;
		bool repeat;

		// These are for debugging only
		EVENT_CLASS_TYPE(TextInput);
		EVENT_INFO_STRING("Unicode character: '%s' Unicode codepoint: U+%x Modifiers: %d Allegro display: %p Repeat: %s",
			character, codepoint, modifiers, display, repeat ? "true" : "false");
	};
	
	class MouseButtonPressedEvent : public Battery::Event {
	public:
		MouseButtonPressedEvent() {}

		void Load(ALLEGRO_EVENT* event);

		int x, y, z, w;
		unsigned int button;
		float pressure;
		ALLEGRO_DISPLAY* display;

		// These are for debugging only
		EVENT_CLASS_TYPE(MouseButtonPressed);
		EVENT_INFO_STRING(" x=%d y=%d z=%d w=%d button=%d pressure=%f display=%p",
			x, y, z, w, button, pressure, display
		);
	};

	class MouseButtonReleasedEvent : public Battery::Event {
	public:
		MouseButtonReleasedEvent() {}

		void Load(ALLEGRO_EVENT* event);

		int x, y, z, w;
		unsigned int button;
		float pressure;
		ALLEGRO_DISPLAY* display;

		// These are for debugging only
		EVENT_CLASS_TYPE(MouseButtonReleased);
		EVENT_INFO_STRING(" x=%d y=%d z=%d w=%d button=%d pressure=%f display=%p",
			x, y, z, w, button, pressure, display
		);
	};

	class MouseMovedEvent : public Battery::Event {
	public:
		MouseMovedEvent() {}

		void Load(ALLEGRO_EVENT* event);

		int x, y;
		int dx, dy;
		float pressure;
		ALLEGRO_DISPLAY* display;

		// These are for debugging only
		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_INFO_STRING(" x=%d y=%d dx=%d dy=%d pressure=%f display=%p",
			x, y, dx, dy, pressure, display
		);
	};

	class MouseScrolledEvent : public Battery::Event {
	public:
		MouseScrolledEvent() {}

		void Load(ALLEGRO_EVENT* event);

		int x, y;
		int dx, dy;
		float pressure;
		ALLEGRO_DISPLAY* display;

		// These are for debugging only
		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_INFO_STRING(" x=%d y=%d dx=%d dy=%d pressure=%f display=%p",
			x, y, dx, dy, pressure, display
		);
	};

	class PureAllegroEvent : public Battery::Event {
	public:
		PureAllegroEvent() {}

		void Load(ALLEGRO_EVENT* event);

		// These are for debugging only
		EVENT_CLASS_TYPE(Event not handled by Battery);
		EVENT_INFO_STRING("");
	};

	class ParentEventContainer {
	public:
		ParentEventContainer() {}

		void Load(ALLEGRO_EVENT* allegroEvent);

		WindowCloseEvent			windowCloseEvent;
		WindowResizeEvent			windowResizeEvent;
		WindowFocusEvent			windowFocusEvent;
		WindowLostFocusEvent		windowLostFocusEvent;
		WindowMouseEnteredEvent		windowMouseEnteredEvent;
		WindowMouseLeftEvent		windowMouseLeftEvent;
		MouseButtonPressedEvent		mouseButtonPressedEvent;
		MouseButtonReleasedEvent	mouseButtonReleasedEvent;
		KeyPressedEvent				keyPressedEvent;
		KeyReleasedEvent			keyReleasedEvent;
		TextInputEvent				textInputEvent;
		MouseMovedEvent				mouseMovedEvent;
		MouseScrolledEvent			mouseScrolledEvent;
		PureAllegroEvent			pureAllegroEvent;

		enum EventType primaryEventType = Battery::EventType::NONE;
		enum EventType secondaryEventType = Battery::EventType::NONE;
		Battery::Event* primaryEvent = nullptr;
		Battery::Event* secondaryEvent = nullptr;
	};

}
