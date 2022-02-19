#pragma once

#include "Battery/Core/Config.h"

#ifdef BATTERY_DEBUG
#define EVENT_CLASS_TYPE_BASE(type) virtual const char* GetTypeString() { return #type; }
#define EVENT_CLASS_TYPE(type) const char* GetTypeString() override { return #type; }
#define EVENT_INFO_STRING_BASE(__format, ...) virtual void GetInfoString(char* buffer, size_t length) { \
	snprintf(buffer, length, "%s: " __format, GetTypeString(), __VA_ARGS__);	\
}
#define EVENT_INFO_STRING(__format, ...) void GetInfoString(char* buffer, size_t length) override { \
	snprintf(buffer, length, "%s: " __format, GetTypeString(), __VA_ARGS__);	\
}
#else
#define EVENT_CLASS_TYPE_BASE(type)
#define EVENT_CLASS_TYPE(type)
#define EVENT_INFO_STRING_BASE(__format, ...)
#define EVENT_INFO_STRING(__format, ...)
#endif

namespace Battery {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMouseEntered, WindowMouseLeft, // WindowMoved,
		KeyPressed, KeyReleased, TextInput,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		PureAllegro
	};

	class Event {
	public:
		Event();
		virtual ~Event();

		void Load(enum class EventType type, ALLEGRO_EVENT* allegroEvent);
		
		enum class EventType GetType();
		bool WasHandled();
		void SetHandled(bool h = true);
		ALLEGRO_EVENT* GetAllegroEvent();

		// These are for debugging only
		EVENT_CLASS_TYPE_BASE(None);
		EVENT_INFO_STRING_BASE("No specific event data");

	private:
		enum class EventType eventType = EventType::None;
		bool handled = false;
		ALLEGRO_EVENT* event;
	};

}
