
#include "pch.h"
#include "BatteryApp.h"
#include "Battery/Extern/magic_enum.hpp"

#include "windows.h"
#include "shellapi.h"

void BatteryApp::OnStartup() {
    using namespace Battery;

    auto im = Battery::DecodeBase64(BATTERY_DEFAULT_WINDOW_ICON_BASE64);
    sf::Image image;
    image.loadFromMemory(&im[0], im.size());

    tray = std::make_unique<Battery::TrayIcon>(image, "Quick tooltip");

    tray->attachLeftClickCallback([&] { LOG_DEBUG("Left click"); });
    tray->attachRightClickCallback([&] { LOG_DEBUG("Right click"); });
}

void BatteryApp::OnRender() {

    tray->update();

    ImGui::ShowDemoWindow();
    
}
