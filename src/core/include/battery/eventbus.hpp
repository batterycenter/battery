#pragma once

#include "dexode/EventBus.hpp"

namespace b {

    class EventBus {
    public:

        EventBus() : m_eventbus(std::make_shared<dexode::EventBus>()), m_eventListener(m_eventbus) {}

        template<typename T, typename... TArgs>
        bool dispatchEvent(TArgs&&... args) {
            if (!m_eventListener.isListening<T>()) {
                return false;
            }
            m_eventbus->postpone<T>({ std::forward<TArgs>(args)... });
            m_eventbus->process();
            return true;
        }

        template<typename T>
        void attachEventHandler(std::function<void(const T&)> listener) {
            m_eventListener.listenToCallback(listener);
        }

        // Prevent all copy and move operations to prevent issues with lambda callbacks
        EventBus(const EventBus&) = delete;
        EventBus(EventBus&&) = delete;
        EventBus& operator=(const EventBus&) = delete;
        EventBus& operator=(EventBus&&) = delete;

    private:
        std::shared_ptr<dexode::EventBus> m_eventbus;
        dexode::EventBus::Listener m_eventListener;
    };

} // namespace b