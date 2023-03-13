
#include "battery/core/environment.h"
#include "battery/core/main.h"
#include "battery/core/string.h"
#include "battery/core/log.h"
#include "battery/core/platform.h"
#include "battery/core/exception.h"

#include "battery/core/internal/windows.h"

namespace battery {

    static std::vector<std::string> parse_cli([[maybe_unused]] int argc, [[maybe_unused]] const char** argv) {
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
            args.push_back(battery::string::osstring_to_utf8(_args[i]));
        }
        ::LocalFree(_args);

        return args;
#else
        return { argv, argv + argc };    // All other platforms simply pass-through
#endif
    }

    static void setup_windows_console() {
#ifdef BATTERY_ARCH_WINDOWS
        SetConsoleOutputCP(CP_UTF8);
        battery::log::core::trace("Switched terminal codepage to Unicode (UTF-8)");
#endif
    }
	
    const char** args_to_cstr(const std::vector<std::string>& args) {
        static auto args_raw = args;     // Do this so the data will stay in memory forever (needed by c_str())
        static std::vector<const char*> arguments;
        
		for (const auto& arg : args_raw) {
			arguments.push_back(arg.c_str());
		}
        return arguments.data();
    }

    int run_main(int argc, const char** argv) {
        int result = 0;
        setup_windows_console();
        catch_common_exceptions([&result, argc, argv]() {
            result = battery::battery_main(parse_cli(argc, argv));
        });
        return result;
    }
}

#ifndef BATTERY_CORE_NO_MAIN
int main(int argc, const char** argv) {     // This is the global entry point, the program starts here
    return battery::run_main(argc, argv);
}
#endif