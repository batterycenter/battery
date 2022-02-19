
#include "Battery/pch.h"
#include "Battery/Core/Event.h"
#include "Battery/Log/Log.h"

namespace Battery {

	Event::Event() {
	}

	Event::~Event() {

	}

	void Event::Load(enum class EventType type, ALLEGRO_EVENT* allegroEvent) {
		eventType = type;
		event = allegroEvent;
		handled = false;

		if (event == nullptr) {
			LOG_CORE_WARN(__FUNCTION__"(): The supplied Allegro Event pointer is nullptr");
		}
	}

	enum class EventType Event::GetType() {
		return eventType;
	}

	bool Event::WasHandled() {
		return handled;
	}

	void Event::SetHandled(bool h) {
		handled = h;
	}

	ALLEGRO_EVENT* Event::GetAllegroEvent() {
		return event;
	}

}