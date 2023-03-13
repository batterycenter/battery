#pragma once

#include "battery/core/thread.h"
#include <mutex>

namespace battery {

    class detachable_thread {   // TODO: Rework this class using std::async and std::future
    public:
        detachable_thread() = default;

        template<typename T>
        void execute_sync(T function) const {
            function();
        }

        template<typename _Fn, typename... _Args>
        void execute_sync_thread(_Fn&& function, _Args&&... args) {
            if (thread.joinable()) {
                thread.join();
            }
            thread = battery::thread(std::forward<_Fn>(function), args...);
            thread.join();
        }

        template<typename _Fn, typename... _Args>
        void execute_async_thread(_Fn&& function, _Args&&... args) {
            if (thread.joinable()) {
                thread.join();
            }
            thread = battery::thread(std::forward<_Fn>(function), args...);
        }

        void join() {
            thread.join();
        }

    private:
        battery::thread thread;
    };

}