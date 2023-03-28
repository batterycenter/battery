
#include "battery/core/environment.hpp"
#include "battery/core/signal.hpp"
#include "battery/core/log.hpp"
#include "battery/core/time.hpp"
#include "battery/core/thread.hpp"

#include <queue>

#ifdef BATTERY_ARCH_WINDOWS
#include <Windows.h>
#else
#include <signal.h>
#endif

namespace b {

    namespace internal {

        class signal_dispatcher {
        public:
            signal_dispatcher() = default;
            ~signal_dispatcher() {
                terminate = true;
                if (worker.joinable()) {
                    worker.join();
                }
            };

            void push(const std::function<void()>& func) {
                std::scoped_lock lock(mutex);
                queue.push(func);
            }

            void start_dispatcher_thread() {
                worker = b::thread([this]() {
                    while (!terminate) {
                        std::function<void()> func;
                        {
                            std::scoped_lock lock(mutex);
                            if (queue.empty()) {
                                continue;
                            }
                            func = queue.front();
                            queue.pop();
                        }
                        if (func) {
                            func();
                        }
                    }
                });
            }

        private:
            std::atomic<bool> terminate = false;
            std::mutex mutex;
            b::thread worker;
            std::queue<std::function<void()>> queue;
        };

        signal_dispatcher signal_dispatcher;
        std::function<void()> ctrl_c_handler_callback = []() {
            b::print("BATTERY_CTRL_C 1");
            b::sleep(1);
            b::print("BATTERY_CTRL_C 2");
            b::sleep(1);
            b::print("BATTERY_CTRL_C 3");
            b::sleep(1);
            b::print("BATTERY_CTRL_C Done");
        };
    }

    void set_ctrl_c_handler(const std::function<void()>& handler) {
        internal::ctrl_c_handler_callback = handler;
    }

    void internal::init_ctrl_c_handler() {
#ifdef BATTERY_ARCH_WINDOWS
        SetConsoleCtrlHandler(nullptr, FALSE);          // Reset to default
        SetConsoleCtrlHandler([](DWORD dwCtrlType) {
            switch (dwCtrlType) {
                case CTRL_C_EVENT:
                case CTRL_BREAK_EVENT:
                    signal_dispatcher.push([]() {
                        if (internal::ctrl_c_handler_callback) {
                            internal::ctrl_c_handler_callback();
                        }
                    });
                    return TRUE;
                default:
                    return FALSE;
            }
        }, TRUE);
#else
        struct sigaction sigIntHandler;
        sigIntHandler.sa_handler = [](int) {
            signal_dispatcher.push([]() {
                if (internal::ctrl_c_handler_callback) {
                    internal::ctrl_c_handler_callback();
                }
            });
        };
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;
        sigaction(SIGINT, &sigIntHandler, nullptr);
#endif
        signal_dispatcher.start_dispatcher_thread();
    }

}
