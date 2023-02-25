
#include "battery/core/Application.h"
#include "battery/core/environment.h"
#include "battery/core/platform.h"
#include "battery/core/log.h"

#include "battery/core/internal/windows.h"

namespace battery {

Application::Application() {

}

Application::~Application() {

}

void Application::run(int argc, const char** argv, const std::string& app_name) {
    SetupWindowsConsole();
    this->app_name = app_name;
    this->cli_args = ParseCLI(argc, argv);
}

std::vector<std::string> Application::ParseCLI(int argc, const char** argv) {
#ifdef BATTERY_ARCH_WINDOWS                 // On Windows, parse CLI arguments from WinAPI and convert to UTF-8
    int _argc = 0;
    LPWSTR* _args = ::CommandLineToArgvW((LPCWSTR)GetCommandLineW(), &_argc);
    if (_args == nullptr) {
        battery::log::core::critical("CommandLineToArgvW failed: {}", battery::get_last_win32_error());
        battery::message_box_error(std::string("[Battery->WinAPI]: CommandLineToArgvW failed: ") + battery::get_last_win32_error());
        return {};
    }
    std::vector<std::string> args;
    for (int i = 0; i < _argc; i++) {
        args.push_back(battery::OsString(_args[i]));
    }
    ::LocalFree(_args);

    return args;
#else
    return { argv, argv + argc };    // All other platforms simple pass-through
#endif
}

void Application::SetupWindowsConsole() {
#ifdef BATTERY_ARCH_WINDOWS
    SetConsoleOutputCP(CP_UTF8);
    battery::log::core::trace("Switched terminal codepage to Unicode (UTF-8)");
#endif
}

}   // namespace battery
