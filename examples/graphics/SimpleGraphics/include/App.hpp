#pragma once

#include "battery/battery.hpp"
#include "battery/embed.hpp"
#include "imgui_internal.h"

class App : public b::Application {
public:
    App() = default;
    lua_State* luaState;
    std::string lua;
    bool fileChanged = false;

    void onSetup() override {
        window->attachEventHandler<b::WindowMovedEvent>([](const auto& event) {
            b::log::info("Window moved to {}, {}", event.position.x, event.position.y);
        });
        window->attachEventHandler<b::WindowCloseEvent>([&](const auto&  /*event*/) {
            b::log::info("Window close");
            close();
        });

        bindEmbeddedLuaScript<"ui/main.lua">();
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
