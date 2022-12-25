
#include "Battery/pch.hpp"
#include "Battery/Core/Log.h"
#include "Battery/Utils/OsString.h"
#include "Battery/Core/Main.h"
#include "Battery/Platform/Platform.h"

#ifdef BATTERY_ARCH_WINDOWS
#include <windows.h>
#include <shellapi.h> // for CommandLineToArgvW
#endif

namespace Battery {
	
    const char** ArgsToCStr(const Args_t& _args) {
        static Args_t args_raw = _args;     // Do this so the data will stay in memory forever (needed by c_str())
        static std::vector<const char*> arguments;
        
		for (auto& arg : args_raw) {
			arguments.push_back(arg.c_str());
		}
        return arguments.data();
    }
		
}

// This is the main entry point for everything
int main(int argc, const char** argv) {

#ifdef BATTERY_ARCH_WINDOWS
    std::system("chcp 65001>NUL");
    Battery::Log::Core::Trace("Switched terminal codepage to Unicode (UTF-8)");

    // On Windows, parse CLI arguments from WinAPI and convert to UTF-8
    int _argc = 0;
    LPWSTR* _args = ::CommandLineToArgvW((LPCWSTR)GetCommandLineW(), &_argc);
    if (_args == nullptr) {
        Battery::Log::Critical("CommandLineToArgvW failed: {}", Battery::GetLastWin32ErrorString());
        return -1;      // TODO: Return condition
    }
    std::vector<std::string> args;
    for (int i = 0; i < _argc; i++) {
        args.push_back(Battery::OsString(_args[i]));
    }
    ::LocalFree(_args);

#else
    // On any non-Windows system, just forward the values
    std::vector<std::string> args(argv, argv + argc);
#endif

    // And now call the actual entry point, defined by the user
    return Battery::Main(args);
}
