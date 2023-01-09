
#include "battery/core/environment.h"
#include "battery/core/process.h"
#include "battery/core/exception.h"
#include "battery/core/log.h"

#include <windows.h>

namespace battery {

    process::process() {

    }

    process::~process() {
        if (thread.joinable()) {
            log::core::error("battery::~process(): The process is still running. Detaching the thread now. You must join the process explicitly");
            thread.detach();
        }
    }

    void process::execute() {
        std::unique_lock<std::mutex> lock(func_mutex);

        if (thread.joinable())
            throw battery::exception("battery::process::execute_sync(): Process is still running. Please join before executing again");

        thread = battery::thread([&] { run_process(); });
        thread.join();
    }

    void process::execute_async() {
        std::unique_lock<std::mutex> lock(func_mutex);

        if (thread.joinable())
            throw battery::exception("battery::process::execute_async(): Process is still running. Please join before executing again");

        thread = battery::thread([&] { run_process(); });
    }

    void process::join() {
        std::unique_lock<std::mutex> lock(func_mutex);

        if (!thread.joinable())
            throw battery::exception("battery::process::join(): Cannot join process, there is nothing to join. Did you intend to use .execute_async() instead of .execute()?");

        thread.join();
    }

    void process::run_process() {

#ifdef BATTERY_ARCH_WINDOWS
        STARTUPINFOW si;
        ZeroMemory( &si, sizeof(si) );  // Write zeros over the entire struct because the WinAPI does not initialize variables
        si.cb = sizeof(si);

        PROCESS_INFORMATION pi;
        ZeroMemory( &pi, sizeof(pi) );  // Write zeros over the entire struct because the WinAPI does not initialize variables

        std::string cmd = executable.to_string() + " "; // Forge a single string to be called as a command line
        for (auto& arg : args) {
            if (cmd.contains(" ")) {
                cmd += "\"" + arg + "\" ";
            }
            else {
                cmd += arg + " ";
            }
        }
        cmd.pop_back();
        OsString os_cmd = cmd;

        if (!CreateProcess( NULL,           // No module name (use command line)
                            (LPWSTR)os_cmd.wstr().c_str(),
                            NULL,           // Process handle not inheritable
                            NULL,           // Thread handle not inheritable
                            FALSE,          // Set handle inheritance to FALSE
                            0,              // No creation flags
                            NULL,           // Use parent's environment block
                            OsString(working_directory.to_string()).wstr().c_str(),
                            &si,            // Pointer to STARTUPINFO structure
                            &pi )           // Pointer to PROCESS_INFORMATION structure
                )
        {
            error = std::make_pair(GetLastError(), battery::get_last_win32_error());
            return;
        }

        WaitForSingleObject( pi.hProcess, INFINITE );

        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );
#else
#error Architecture not supported yet!
#endif

    }

}