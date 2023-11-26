
#include "battery/string.hpp"
#include "battery/log.hpp"
#include "battery/folders.hpp"
#include "battery/messages.hpp"
#include "battery/time.hpp"
#include "battery/thread.hpp"
#include "battery/internal/windows.hpp"

namespace b {

    static std::vector<std::string> parse_cli([[maybe_unused]] int argc, [[maybe_unused]] const char** argv) {
        std::vector<std::string> args;
#ifdef B_OS_WINDOWS                 // On Windows, parse CLI arguments from WinAPI and convert to UTF-8
        int _argc = 0;
        LPWSTR* _args = ::CommandLineToArgvW((LPCWSTR)GetCommandLineW(), &_argc);
        if (_args == nullptr) {
            b::log::core::critical("CommandLineToArgvW failed: {}", b::internal::get_last_win32_error());
            b::message_box_error("[Battery->WinAPI]: CommandLineToArgvW failed: " + b::internal::get_last_win32_error());
            return {};
        }
        for (int i = 0; i < _argc; i++) {
            args.push_back(b::narrow(std::wstring(_args[i])));
        }
        ::LocalFree(_args);

        return args;
#else                   // All other platforms simply pass-through
        for (int i = 0; i < argc; i++) {
            args.emplace_back(argv[i]);     // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        }
#endif
        return args;
    }

    static void setup_windows_console() {
#ifdef B_OS_WINDOWS
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        b::log::core::trace("Switched terminal codepage to Unicode (UTF-8)");
#endif
    }

    static void set_windows_dpi_awareness() {
#ifdef B_OS_WINDOWS
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        b::log::core::trace("Enabling DPI-awareness");
#endif
    }

    static void print_production_warning() {
#ifndef B_PRODUCTION_MODE
        b::log::core::warn("Battery is running in non-production mode, enabling live-reloaded resources. Make sure to enable production mode before releasing your app!");
#endif
    }
	
    const char** args_to_argv(const std::vector<std::string>& args) {
        static std::vector<std::string> argsData;  // Static, so the data will stay in memory forever (needed by c_str())
        static std::vector<const char*> arguments;

        // Store the actual data into argsData
        argsData.clear();
        for (const auto& arg : args) {
            argsData.push_back(arg);
        }

        // Now get pointers to each of them
        arguments.clear();
		for (const auto& arg : argsData) {
			arguments.push_back(arg.c_str());
		}
        return arguments.data();
    }

    int invoke_main(int argc, const char** argv) {
        b::time();                  // First call sets zero-time to now
        setup_windows_console();
        set_windows_dpi_awareness();
        print_production_warning();

        int result = -1;
        b::thread::catchCommonExceptions([&result, argc, argv]() {
//            result = b::main(parse_cli(argc, argv));
        });
        return result;
    }
}
