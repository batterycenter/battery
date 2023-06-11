#pragma once

#include "battery/core.hpp"
#include "battery/graphics.hpp"

struct MainWindowContext : public b::PyContext {

    std::function<void()> handle_window_dragging;
    bool titlebar_hovered = false;
    ImVec2 window_size;
    sf::Texture steam_logo;
    sf::Texture qrcode_texture;

    bool steam_hyperlink_clicked = false;
    bool help_hyperlink_clicked = false;
    bool create_account_hyperlink_clicked = false;

    B_DEF_PY_CONTEXT(MainWindowContext,
                     handle_window_dragging,
                     titlebar_hovered,
                     window_size,
                     steam_logo,
                     qrcode_texture,
                     steam_hyperlink_clicked,
                     help_hyperlink_clicked,
                     create_account_hyperlink_clicked)
};

class MainWindow : public b::PyWindow<MainWindowContext, "MainWindowContext"> {
public:
    void handle_window_dragging();

    void onAttach() override;
    void onUpdate() override;
    void onDetach() override;

    ~MainWindow() = default;

//    inline static MainWindow& get() { return dynamic_cast<MainWindow&>(*b::py_application::get().windows()[0].get()); }
};

struct AppContext : public b::PyContext {

    bool py_test_property1 = false;
    int py_test_property2 = 0;

    B_DEF_PY_CONTEXT(AppContext, py_test_property1, py_test_property2)
};

class App : public b::PyApplication<AppContext, "AppContext"> {
public:

    MainWindow* s_mainWindow = nullptr;

    App() = default;
    ~App() = default;

    void onSetup() override {
        this->attachWindow<MainWindow>(&s_mainWindow);
    }

    void onUpdate() override {}
    void onCleanup() override {}

    inline static App& get() { return dynamic_cast<App&>(b::BaseApplication::get()); }
};
