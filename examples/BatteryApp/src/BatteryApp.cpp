
#include "pch.h"
#include "BatteryApp.h"
#include "Battery/Extern/magic_enum.hpp"

#include "windows.h"
#include "shellapi.h"

sf::Image sprite_idle;

void BatteryApp::OnStartup() {
    using namespace Battery;

    //sprite_idle.loadFromFile("resources/KnightIdle_strip.png");
    if (!sprite_idle.loadFromFile("resources/arduino16.png"))
        throw UI_EXCEPTION("Oh no");

    auto im = Battery::DecodeBase64(BATTERY_DEFAULT_WINDOW_ICON_BASE64);
    sf::Image image;
    image.loadFromMemory(&im[0], im.size());

    tray = std::make_unique<Battery::TrayIcon>(image, "Quick tooltip");

    tray->attachLeftClickCallback([&] { LOG_DEBUG("Left click"); });
    tray->attachRightClickCallback([&] { LOG_DEBUG("Right click"); });
}

void BatteryApp::OnUpdate() {

    tray->setIcon(sprite_idle);
    //image.loadFrom
    //tray->

}

void BatteryApp::OnRender() {

    tray->update();

    ImGui::ShowDemoWindow();
    
}
