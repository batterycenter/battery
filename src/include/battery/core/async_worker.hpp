#pragma once

#include "battery/core/thread.hpp"
#include <mutex>
#include <future>

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

}