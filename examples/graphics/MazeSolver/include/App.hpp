#pragma once

#include "battery/core.hpp"
#include "battery/graphics.hpp"
#include "MazeHost.hpp"

class MainWindow : public b::BaseWindow {
public:
    MainWindow() = default;

    MazeHost mazeHost;

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

    void onUpdate() override {
        if (!s_mainWindow->isOpen()) {
            stopApplication();
        }
    }
    void onCleanup() override {}
};
