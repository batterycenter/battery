
#include "pch.h"
#include "BatteryApp.h"
#include "Battery/Extern/magic_enum.hpp"

void BatteryApp::OnStartup() {
    using namespace Battery;

    

    CloseApplication();
}

void BatteryApp::OnRender() {
    ImGui::ShowDemoWindow();
    
}
