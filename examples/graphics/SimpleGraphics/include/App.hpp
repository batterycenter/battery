#pragma once

#include "battery/battery.hpp"
#include "battery/embed.hpp"

class App : public b::Application {
public:
    App() = default;

    void onSetup() override {
        window->attachEventHandler<b::WindowMovedEvent>([](const auto& event) {
            b::log::info("Window moved to {}, {}", event.position.x, event.position.y);
        });
        window->attachEventHandler<b::WindowCloseEvent>([&](const auto&  /*event*/) {
            b::log::info("Window close");
            close();
        });

        b::embed<"ui/main.lua">().get([](const auto& file) {
            b::log::info("File: {}", file.str());
        });
    }

    void onUpdate() override {
//        b::println("Frame {}", framecount());
    }

    void onRender() override {
//        b::println("Document: {}", );
    }

    void onClose() override {
        b::log::info("Application closed");
    }
};
