/*
#include "Battery/Core/ApplicationEvents.h"
#include "Battery/Core/AllegroContext.h"

namespace Battery {

	void WindowCloseEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::WindowClose, event);
	}

	void WindowResizeEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::WindowResize, event);
		width = event->display.width;
		height = event->display.height;
		windowPosition = glm::ivec2(event->display.x, event->display.y);
	}

	void WindowFocusEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::WindowFocus, event);
	}

	void WindowLostFocusEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::WindowLostFocus, event);
	}

	void WindowMouseEnteredEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::WindowMouseEntered, event);
	}

	void WindowMouseLeftEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::WindowMouseLeft, event);
	}

	void KeyPressedEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::KeyPressed, event);
		display = event->keyboard.display;
		keycode = event->keyboard.keycode;
		repeat = event->keyboard.repeat;
		modifiers = event->keyboard.modifiers;
	}

	void KeyReleasedEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::KeyReleased, event);
		display = event->keyboard.display;
		keycode = event->keyboard.keycode;
		repeat = event->keyboard.repeat;
		modifiers = event->keyboard.modifiers;
	}

	void TextInputEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::TextInput, event);
		display = event->keyboard.display;
		codepoint = event->keyboard.unichar;
		repeat = event->keyboard.repeat;
		modifiers = event->keyboard.modifiers;
		StringUtils::ConvertCodepointToUTF8(codepoint, character, sizeof(character));
	}

	void MouseButtonPressedEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::MouseButtonPressed, event);
		x = event->mouse.x; y = event->mouse.y; z = event->mouse.z;
		w = event->mouse.w; button = event->mouse.button;
		pressure = event->mouse.pressure; display = event->mouse.display;
	}

	void MouseButtonReleasedEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::MouseButtonReleased, event);
		x = event->mouse.x; y = event->mouse.y; z = event->mouse.z;
		w = event->mouse.w; button = event->mouse.button;
		pressure = event->mouse.pressure; display = event->mouse.display;
	}

	void MouseMovedEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::MouseMoved, event);
		x = event->mouse.x; y = event->mouse.y; dx = event->mouse.dx; dy = event->mouse.dy;
		pressure = event->mouse.pressure; display = event->mouse.display;
	}

	void MouseScrolledEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::MouseScrolled, event);
		x = event->mouse.w; y = event->mouse.z; dx = event->mouse.dw; dy = event->mouse.dz;
		pressure = event->mouse.pressure; display = event->mouse.display;
	}

	void PureAllegroEvent::Load(ALLEGRO_EVENT* event) {
		Battery::Event::Load(EventType::PureAllegro, event);
	}

	void ParentEventContainer::Load(ALLEGRO_EVENT* allegroEvent) {

		primaryEventType = EventType::NONE;
		secondaryEventType = EventType::NONE;
		primaryEvent = nullptr;
		secondaryEvent = nullptr;

		if (allegroEvent == nullptr)
			return;

		switch (allegroEvent->type) {

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			windowCloseEvent.Load(allegroEvent);
			primaryEvent = &windowCloseEvent;
			primaryEventType = EventType::WindowClose;
			return;

		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			windowResizeEvent.Load(allegroEvent);
			primaryEvent = &windowResizeEvent;
			primaryEventType = EventType::WindowResize;
			return;

		case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
			windowFocusEvent.Load(allegroEvent);
			primaryEvent = &windowFocusEvent;
			primaryEventType = EventType::WindowFocus;
			return;

		case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
			windowLostFocusEvent.Load(allegroEvent);
			primaryEvent = &windowLostFocusEvent;
			primaryEventType = EventType::WindowLostFocus;
			return;

		case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
			windowMouseEnteredEvent.Load(allegroEvent);
			primaryEvent = &windowMouseEnteredEvent;
			primaryEventType = EventType::WindowMouseEntered;
			return;

		case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
			windowMouseLeftEvent.Load(allegroEvent);
			primaryEvent = &windowMouseLeftEvent;
			primaryEventType = EventType::WindowMouseLeft;
			return;

		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			mouseButtonPressedEvent.Load(allegroEvent);
			primaryEvent = &mouseButtonPressedEvent;
			primaryEventType = EventType::MouseButtonPressed;
			return;

		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			mouseButtonReleasedEvent.Load(allegroEvent);
			primaryEvent = &mouseButtonReleasedEvent;
			primaryEventType = EventType::MouseButtonReleased;
			return;

		case ALLEGRO_EVENT_KEY_DOWN:
			keyPressedEvent.Load(allegroEvent);
			primaryEvent = &keyPressedEvent;
			primaryEventType = EventType::KeyPressed;
			return;

		case ALLEGRO_EVENT_KEY_UP:
			keyReleasedEvent.Load(allegroEvent);
			primaryEvent = &keyReleasedEvent;
			primaryEventType = EventType::KeyReleased;
			return;

		case ALLEGRO_EVENT_KEY_CHAR:
			textInputEvent.Load(allegroEvent);
			primaryEvent = &textInputEvent;
			primaryEventType = EventType::TextInput;
			return;

		case ALLEGRO_EVENT_MOUSE_AXES:

			if (allegroEvent->mouse.dx != 0 || allegroEvent->mouse.dy != 0) {
				mouseMovedEvent.Load(allegroEvent);
				primaryEvent = &mouseMovedEvent;
				primaryEventType = EventType::MouseMoved;
			}

			if (allegroEvent->mouse.dz != 0 || allegroEvent->mouse.dw != 0) {
				mouseScrolledEvent.Load(allegroEvent);
				if (primaryEventType == EventType::MouseMoved) {
					secondaryEvent = &mouseScrolledEvent;
					secondaryEventType = EventType::MouseScrolled;
				}
				else {
					primaryEvent = &mouseScrolledEvent;
					primaryEventType = EventType::MouseScrolled;
				}
			}
			return;

		default:
			pureAllegroEvent.Load(allegroEvent);
			primaryEvent = &pureAllegroEvent;
			primaryEventType = EventType::PureAllegro;
			return;
		}
	}

}
*/