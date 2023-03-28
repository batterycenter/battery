
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

            signal_dispatcher(const signal_dispatcher&) = delete;
            signal_dispatcher& operator=(const signal_dispatcher&) = delete;
            signal_dispatcher(signal_dispatcher&& other) = delete;
            signal_dispatcher& operator=(signal_dispatcher&& other) = delete;

            void push(const std::function<void()>& func) {
                std::scoped_lock lock(mutex);
                queue.push(func);
            }

            void start_dispatcher_thread() {
                worker = b::thread([this]() {
                    while (!terminate) {
                        if (auto func = pop_function(); func) {
                            func();
                        }
                        b::sleep(0.1);
                    }
                });
            }

        private:
            std::function<void()> pop_function() {
                std::scoped_lock lock(mutex);
                if (queue.empty()) {
                    return {};
                }
                auto func = queue.front();
                queue.pop();
                return func;
            }

            std::mutex mutex;
            b::thread worker;
            std::atomic<bool> terminate = false;
            std::queue<std::function<void()>> queue;
        };

        inline static signal_dispatcher dispatcher;
        inline static std::function<void()> ctrl_c_handler_callback = []() {
            b::log::core::error("Ctrl+C caught. Please use b::set_ctrl_c_handler() to define a callback that causes b::main() to return");
            std::exit(EXIT_FAILURE);
        };
    }

    void set_ctrl_c_handler(const std::function<void()>& handler) {
        internal::ctrl_c_handler_callback = handler;
    }

    void internal::init_ctrl_c_handler() {
#ifdef BATTERY_ARCH_WINDOWS
        SetConsoleCtrlHandler([](DWORD dwCtrlType) {
            switch (dwCtrlType) {
                case CTRL_C_EVENT:
                case CTRL_BREAK_EVENT:
                    dispatcher.push(internal::ctrl_c_handler_callback);
                    return TRUE;
                default:
                    return FALSE;
            }
        }, TRUE);
#else
        struct sigaction sigIntHandler;                     // We need to call dispatcher.push from a different thread,
        sigIntHandler.sa_handler = [](int) {                // because signals on Unix are interrupts, which can cause
            b::thread([]() {                                // a deadlock in mutexes.
                dispatcher.push(internal::ctrl_c_handler_callback);
            }).detach();
        };
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;
        sigaction(SIGINT, &sigIntHandler, nullptr);
#endif
        dispatcher.start_dispatcher_thread();
    }

}
