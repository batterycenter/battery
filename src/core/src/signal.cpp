
#include "battery/core/environment.hpp"
#include "battery/core/signal.hpp"
#include "battery/core/log.hpp"
#include "battery/core/time.hpp"
#include "battery/core/thread.hpp"

#include <queue>
#include <stack>

#ifdef BATTERY_ARCH_WINDOWS
#include <Windows.h>
#else
#include <signal.h>
#endif

namespace b {

    static void enable_native_signal_handler();
    static void disable_native_signal_handler();

    class SignalHandler {
    public:
        static SignalHandler& getInstance() {
            static SignalHandler instance;
            return instance;
        }

        void push(const std::function<void()>& func) {
            std::scoped_lock lock(stackMutex);
            handlerStack.push(func);
            applyHandler();
        }

        void pop() {
            std::scoped_lock lock(stackMutex);
            if (!handlerStack.empty()) {
                handlerStack.pop();
            }
            applyHandler();
        }

        void applyHandler() {
            if (!handlerStack.empty()) {
                if (!nativeHandlerEnabled) {
                    enable_native_signal_handler();
                    nativeHandlerEnabled = true;
                }
            } else {
                if (nativeHandlerEnabled) {
                    disable_native_signal_handler();
                    nativeHandlerEnabled = false;
                }
            }
        }

        static void pushCtrlCEvent() {      // This is called in the platform native handler. But it must be called
            auto& instance = getInstance();                     // from a thread, not an interrupt!!!!
            std::scoped_lock stackLock(instance.stackMutex, instance.eventMutex);
            if (!instance.handlerStack.empty()) {
                instance.eventQueue.push(instance.handlerStack.top());
            }
        }

        SignalHandler(const SignalHandler&) = delete;
        SignalHandler& operator=(const SignalHandler&) = delete;
        SignalHandler(SignalHandler&& other) = delete;
        SignalHandler& operator=(SignalHandler&& other) = delete;

    private:
        SignalHandler() {
            eventHandlerThread = b::thread(&SignalHandler::eventHandler, this);
        }

        std::function<void()> popEvent() {
            std::scoped_lock lock(eventMutex);
            if (eventQueue.empty()) {
                return {};
            }
            auto function = eventQueue.front();
            eventQueue.pop();
            return function;
        }

        void eventHandler() {
            while (!eventHandlerThread.get_stop_token().stop_requested()) {
                std::function<void()> callback = popEvent();
                if (callback) {
                    callback();
                } else {
                    b::sleep(0.1);
                }
            }
        }

        std::mutex stackMutex;
        std::mutex eventMutex;
        std::stack<std::function<void()>> handlerStack;
        std::queue<std::function<void()>> eventQueue;
        std::atomic<bool> nativeHandlerEnabled = false;
        b::thread eventHandlerThread;
    };

#ifdef BATTERY_ARCH_WINDOWS
    static int windows_handler(DWORD dwCtrlType) {  // Separate function instead of lambda: We need the function pointer twice
        switch (dwCtrlType) {
            case CTRL_C_EVENT:
            case CTRL_BREAK_EVENT:
                SignalHandler::pushCtrlCEvent();
                return TRUE;
            default:
                return FALSE;
        }
    }

    static void enable_native_signal_handler() {
        SetConsoleCtrlHandler(windows_handler, TRUE);
    }

    static void disable_native_signal_handler() {
        SetConsoleCtrlHandler(windows_handler, FALSE);
    }
#else
    static void enable_native_signal_handler() {
        struct sigaction sigIntHandler;                     // We need to call push from a different thread,
        sigIntHandler.sa_handler = [](int) {                // because signals on Unix are interrupts, which will cause
            b::thread([]() {                                // a deadlock in mutexes.
                SignalHandler::pushCtrlCEvent();
            }).detach();
        };
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;
        sigaction(SIGINT, &sigIntHandler, nullptr);
    }

    static void disable_native_signal_handler() {
        struct sigaction sigIntHandler;
        sigIntHandler.sa_handler = SIG_DFL;
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;
        sigaction(SIGINT, &sigIntHandler, nullptr);
    }
#endif

    void push_ctrl_c_handler(const std::function<void()>& handler) {
        auto& signalhandler = SignalHandler::getInstance();
        signalhandler.push(handler);
    }

    void pop_ctrl_c_handler() {
        auto& signalhandler = SignalHandler::getInstance();
        signalhandler.pop();
    }

    void generate_ctrl_c_event() {
#ifdef BATTERY_ARCH_WINDOWS
        GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
#else
        raise(SIGINT);
#endif
    }

} // namespace b