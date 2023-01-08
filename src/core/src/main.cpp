
#include "battery/core/common.h"
#include "battery/core/main.h"
#include "battery/core/platform.h"
#include "battery/core/OsString.h"

namespace battery {
	
    const char** args_to_cstr(const Args_t& _args) {
        static Args_t args_raw = _args;     // Do this so the data will stay in memory forever (needed by c_str())
        static std::vector<const char*> arguments;
        
		for (auto& arg : args_raw) {
			arguments.push_back(arg.c_str());
		}
        return arguments.data();
    }
		
}

#ifdef BATTERY_ARCH_WINDOWS
#include <windows.h>
#include <shellapi.h> // for CommandLineToArgvW

static std::vector<std::string> retrieve_windows_args() {
    // On Windows, parse CLI arguments from WinAPI and convert to UTF-8
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
}
#endif



// This is the main entry point for everything
int main(int argc, const char** argv) {
    std::vector<std::string> args;

#ifdef BATTERY_ARCH_WINDOWS
    SetConsoleOutputCP(CP_UTF8);
    battery::log::core::trace("Switched terminal codepage to Unicode (UTF-8)");

    args = retrieve_windows_args();
    if (args.empty()) return -1;
#else
    // On any non-Windows system, just forward the values
    args = { argv, argv + argc };
#endif

    // And now call the actual entry point, defined by the user
    return battery::main(args);
}
