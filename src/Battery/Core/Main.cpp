
#include "Battery/pch.hpp"
#include "Battery/Core/Log.h"
#include "Battery/Utils/OsString.h"
#include "Battery/Core/Main.h"
#include "Battery/Platform/Platform.h"

#ifdef BATTERY_ARCH_WINDOWS
#include <windows.h>
#include <shellapi.h> // for CommandLineToArgvW
#endif

// This is the main entry point for everything
int main(int argc, const char** argv) {

#ifdef BATTERY_ARCH_WINDOWS
    system("chcp 65001>NUL");
    Battery::Log::Core::Trace("Switched terminal codepage to Unicode (UTF-8)");

    // On Windows, parse CLI arguments from WinAPI and convert to UTF-8
    LPWSTR* _args;
    int _argc = 0;
    _args = CommandLineToArgvW((LPCWSTR)GetCommandLineW(), &_argc);
    if (!_args) {
        std::cerr << "CommandLineToArgvW failed: " << Battery::GetLastWin32ErrorString() << std::endl;
        return -1;      // TODO
    }
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++) {
        args.push_back(Battery::OsString(_args[i]));
    }

#else
    // On any non-Windows system, just forward the values
    std::vector<std::string> args(argv, argv + argc);
#endif

    // And now call the actual entry point, defined by the user
    return Battery::Main(args);

}
