
#define PRINT_PLATFORMS
#include "battery/core/environment.hpp"

#include "battery/core/main.hpp"
#include "battery/core/string.hpp"
#include "battery/core/signal.hpp"
#include "battery/core/log.hpp"
#include "battery/core/folders.hpp"
#include "battery/core/messages.hpp"
#include "battery/core/time.hpp"
#include "battery/core/thread.hpp"
#include "battery/core/internal/windows.hpp"

namespace b {

    static std::vector<std::string> parse_cli([[maybe_unused]] int argc, [[maybe_unused]] const char** argv) {
#ifdef BATTERY_ARCH_WINDOWS                 // On Windows, parse CLI arguments from WinAPI and convert to UTF-8
        int _argc = 0;
        LPWSTR* _args = ::CommandLineToArgvW((LPCWSTR)GetCommandLineW(), &_argc);
        if (_args == nullptr) {
            b::log::core::critical("CommandLineToArgvW failed: {}", b::get_last_win32_error());
            b::message_box_error(std::string("[Battery->WinAPI]: CommandLineToArgvW failed: ") + b::get_last_win32_error());
            return {};
        }
        std::vector<std::string> args;
        for (int i = 0; i < _argc; i++) {
            args.push_back(b::from_osstring(_args[i]));
        }
        ::LocalFree(_args);

        return args;
#else
        return { argv, argv + argc };    // All other platforms simply pass-through
#endif
    }

    static void setup_windows_console() {
#ifdef BATTERY_ARCH_WINDOWS
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        b::log::core::trace("Switched terminal codepage to Unicode (UTF-8)");
#endif
    }
	
    const char** args_to_argv(const std::vector<std::string>& args) {
        static auto args_raw = args;     // Static, so the data will stay in memory forever (needed by c_str())
        static std::vector<const char*> arguments;
        
		for (const auto& arg : args_raw) {
			arguments.push_back(arg.c_str());
		}
        return arguments.data();
    }

    int run_main(int argc, const char** argv) {
        b::internal::reset_time();
        setup_windows_console();
        b::internal::init_ctrl_c_handler();

        int result = -1;
        b::thread::catch_common_exceptions([&result, argc, argv]() {
            result = b::main(parse_cli(argc, argv));
        });
        return result;
    }
}

#ifndef BATTERY_CORE_NO_MAIN

int main(int argc, const char** argv) {     // This is the global entry point, the program starts here
    return b::run_main(argc, argv);
}

#endif