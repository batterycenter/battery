
#include "battery/core/environment.hpp"
#include "battery/core/signal.hpp"
#include "battery/core/log.hpp"
#include "battery/core/time.hpp"

#ifdef BATTERY_ARCH_WINDOWS
#include <Windows.h>
#else
#include <signal.h>
#endif

namespace b {

    void set_ctrl_c_handler(const std::function<void()>& handler) {
#ifdef BATTERY_ARCH_WINDOWS
        SetConsoleCtrlHandler(nullptr, FALSE);          // Reset to default
        SetConsoleCtrlHandler([](DWORD dwCtrlType) {
            switch (dwCtrlType) {
                case CTRL_C_EVENT:
                case CTRL_BREAK_EVENT:
                    if (internal::ctrl_c_handler_callback) {
                        internal::ctrl_c_handler_callback();
                    }
                    return TRUE;
                default:
                    return FALSE;
            }
        }, TRUE);
#else
        struct sigaction sigIntHandler;
        sigIntHandler.sa_handler = my_handler;
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;
#endif
    }

    void internal::init_ctrl_c_handler() {
        internal::ctrl_c_handler_callback = []() {
            b::print("BATTERY_CTRL_C 1");
            b::sleep(1);
            b::print("BATTERY_CTRL_C 2");
            b::sleep(1);
            b::print("BATTERY_CTRL_C 3");
            b::sleep(1);
            b::print("BATTERY_CTRL_C Done");
        };
        set_ctrl_c_handler(internal::ctrl_c_handler_callback);
    }

}
