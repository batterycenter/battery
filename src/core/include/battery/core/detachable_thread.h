#pragma once

#include "battery/core/thread.h"
#include <mutex>

namespace battery {

    class detachable_thread {
    public:
        detachable_thread() : lock(working_mutex) {};

        detachable_thread(const detachable_thread&) = delete;
        detachable_thread(detachable_thread&&) = delete;
        detachable_thread& operator=(const detachable_thread&) = delete;    // We need to delete all of these
        detachable_thread& operator=(detachable_thread&& other) = delete;   //  as we cannot std::move our std::mutex

        template<typename T>
        void execute_sync(T function) {
            std::scoped_lock scoped_lock(working_mutex);
            function();
        }

        template<typename _Fn, typename... _Args>
        void execute_sync_thread(_Fn&& function, _Args&&... args) {
            std::scoped_lock scoped_lock(working_mutex);
            thread = battery::thread(std::forward<_Fn>(function), args...);
            thread.join();
        }

        template<typename _Fn, typename... _Args>
        void execute_async_thread(_Fn&& function, _Args&&... args) {
            lock.lock();
            thread = battery::thread(std::forward<_Fn>(function), args...);
        }

        void join() {
            thread.join();
            lock.unlock();
        }

    private:
        battery::thread thread;
        std::unique_lock<std::mutex> lock;
        std::mutex working_mutex;
    };

}