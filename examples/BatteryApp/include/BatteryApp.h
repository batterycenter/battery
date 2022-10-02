
#include "pch.h"

class BatteryApp : public Battery::Application {
public:
    BatteryApp() {}

    std::unique_ptr<Battery::TrayIcon> tray;

    void OnStartup() override;
    //void OnUpdate() override;
    void OnRender() override;
    //void OnShutdown() override;
};
