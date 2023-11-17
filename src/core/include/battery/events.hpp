#pragma once

#include "battery/vec.hpp"
#include <queue>

namespace b {

    struct Event {};

    struct ApplicationQuitEvent : public Event {};          // SDL_QUIT
    struct WindowCloseEvent : public Event {};              // SDL_WINDOWEVENT_CLOSE
    struct WindowResizeEvent : public Event {               // SDL_WINDOWEVENT_RESIZED
        b::Vec2 size;
        WindowResizeEvent() = default;
        WindowResizeEvent(b::Vec2 size) : size(size) {}
    };
    struct WindowLostFocusEvent : public Event {};          // SDL_WINDOWEVENT_FOCUS_LOST
    struct WindowGainedFocusEvent : public Event {};        // SDL_WINDOWEVENT_FOCUS_GAINED
    struct WindowMaximizedEvent : public Event {};          // SDL_WINDOWEVENT_MAXIMIZED
    struct WindowMinimizedEvent : public Event {};          // SDL_WINDOWEVENT_MINIMIZED
    struct WindowRestoredEvent : public Event {};           // SDL_WINDOWEVENT_RESTORED
    struct WindowMovedEvent : public Event {                // SDL_WINDOWEVENT_MOVED
        b::Vec2 position;
        WindowMovedEvent() = default;
        WindowMovedEvent(b::Vec2 position) : position(position) {}
    };
    struct WindowHiddenEvent : public Event {};             // SDL_WINDOWEVENT_HIDDEN
    struct WindowShownEvent : public Event {};              // SDL_WINDOWEVENT_SHOWN
    struct WindowExposedEvent : public Event {};            // SDL_WINDOWEVENT_EXPOSED
    struct MouseEnteredWindowEvent : public Event {};       // SDL_WINDOWEVENT_ENTER
    struct MouseLeftWindowEvent : public Event {};          // SDL_WINDOWEVENT_LEAVE


//    namespace Events {
//        struct TextEnteredEvent : public sf::Event::TextEvent {};
//        struct KeyPressEvent : public sf::Event::KeyEvent {};
//        struct KeyReleaseEvent : public sf::Event::KeyEvent {};
//        struct MouseWheelScrollEvent {
//            b::Vec2 scrollDelta;
//        };
//        struct MouseButtonPressEvent : public sf::Event::MouseButtonEvent {};
//        struct MouseButtonReleaseEvent : public sf::Event::MouseButtonEvent {};
//        struct MouseMoveEvent : public MousePositionData {};
//        struct MouseEnteredWindowEvent {};
//        struct MouseLeftWindowEvent {};
//        struct JoystickButtonPressEvent : public sf::Event::JoystickButtonEvent {};
//        struct JoystickButtonReleaseEvent : public sf::Event::JoystickButtonEvent {};
//        struct JoystickMoveEvent : public sf::Event::JoystickMoveEvent {};
//        struct JoystickConnectEvent : public sf::Event::JoystickConnectEvent {};
//        struct JoystickDisconnectEvent : public sf::Event::JoystickConnectEvent {};
//        struct TouchBeginEvent : public sf::Event::TouchEvent {};
//        struct TouchMoveEvent : public sf::Event::TouchEvent {};
//        struct TouchEndEvent : public sf::Event::TouchEvent {};
//        struct SensorChangeEvent : public sf::Event::SensorEvent {};
//    }

} // namespace b