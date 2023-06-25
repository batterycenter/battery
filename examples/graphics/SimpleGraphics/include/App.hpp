#pragma once

#include "battery/core/console_application.hpp"
#include "battery/core/log.hpp"
#include "battery/core/main.hpp"
#include "battery/core/all.hpp"
#include "battery/graphics/application.hpp"

class MainWindow : public b::BaseWindow {
public:
    MainWindow() = default;

    sf::Font font;
    sf::Texture battery;
    b::vec2 position = { 100.0f, 100.0f };
    b::vec2 velocity = { 1.0f, 1.0f };

    void onAttach() override;
    void onUpdate() override;
    void onDetach() override;
};

class App : public b::BaseApplication {
public:
    App() = default;

    inline static MainWindow* s_mainWindow;

    void onSetup() override {
        this->attachWindow<MainWindow>(&s_mainWindow);
    }

    void onUpdate() override {}
    void onCleanup() override {}
};
