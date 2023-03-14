#pragma once

#include "battery/core/thread.h"
#include <mutex>
#include <future>

namespace battery {

    template<typename T>
    class async_worker {
    public:
        async_worker() = default;

        template<typename _Fn, typename... _Args>
        T execute_sync(_Fn&& function, _Args&&... args) const {
            return function(args...);
        }

        template<typename _Fn, typename... _Args>
        void execute_async(_Fn&& function, _Args&&... args) {
            if (worker.valid()) {
                worker.wait();
            }
            worker = std::async(std::launch::async, std::forward<_Fn>(function), args...);
        }

        T join() {
            worker.get();
        }

    private:
        std::future<T> worker;
    };

}