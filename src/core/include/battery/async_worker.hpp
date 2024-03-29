#pragma once

#ifndef B_OS_WEB
#include "thread.hpp"
#include <mutex>
#include <future>

//TODO: Implement a way to stop an async worker using a stop token

namespace b {

    template<typename T>
    class async_worker {
    public:
        async_worker() = default;
        ~async_worker() {
            wait();
        };

        async_worker(const async_worker& other) = delete;
        async_worker& operator=(const async_worker& other) = delete;
        async_worker(async_worker&& other) noexcept = default;
        async_worker& operator=(async_worker&& other) noexcept = default;

        template<typename Fn, typename... Args>
        T execute_sync(Fn&& function, Args&&... args) const {
            return function(args...);
        }

        template<typename Fn, typename... Args>
        void execute_async(Fn&& function, Args&&... args) {
            wait();
            worker = b::async(std::forward<Fn>(function), args...);
        }

        T join() {
            worker.get();
        }

        void wait() {
            if (worker.valid()) {
                worker.wait();
            }
        }

    private:
        std::future<T> worker;
    };

} // namespace b

#endif // !B_OS_WEB
