#pragma once

#include "battery/battery.hpp"

class App : public b::Application {
public:
    App() = default;

    void onSetup() override {
        window.attachEventHandler<b::WindowMovedEvent>([](const auto& event) {
            b::log::info("Window moved to {}, {}", event.position.x, event.position.y);
        });
        window.attachEventHandler<b::WindowCloseEvent>([&](const auto&  /*event*/) {
            b::log::info("Window close");
            close();
        });
    }

    void onUpdate() override {
//        b::println("Frame {}", framecount());
    }

    void onClose() override {
        b::log::info("Application closed");
    }
};
