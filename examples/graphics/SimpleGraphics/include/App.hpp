#pragma once

#include "battery/battery.hpp"

class MainWindow : public b::Window {
public:
    MainWindow() = default;

    b::Vec2 position = { 100.0f, 100.0f };
    b::Vec2 velocity = { 1.0f, 1.0f };

    void onAttach() override {}
    void onUpdate() override {}
    void onDetach() override {}
};

class App : public b::Application {
public:
    App() = default;

    MainWindow window;

    void onSetup() override {
        window.create("Test test", { 800, 600 });
    }

    void onUpdate() override {
        window.update();
    }

    void onClose() override {

    }
};
