#pragma once

#include "battery/battery.hpp"
#include "battery/embed.hpp"
#include "battery/imguilua.hpp"
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

        b::embed<"ui/main.lua">().get([this](const auto& file) {
            lua = file.str();
            fileChanged = true;
        });

        luaState = b::ImGuiLua::CreateLuaState();
    }

    void onUpdate() override {
//        b::println("Frame {}", framecount());

        if (fileChanged) {
            b::log::info("Script reloaded");
            b::ImGuiLua::RunLuaString(luaState, lua);
            fileChanged = false;
        }
    }

    void onRender() override {
        b::ImGuiLua::CallLuaFunction(luaState, "render");
        ImGui::ErrorCheckEndFrameRecover(nullptr);
//        b::println("Document: {}", );
    }

    void onClose() override {
        b::log::info("Application closed");
    }
};
