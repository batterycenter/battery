#pragma once

#include <cstdint>
#include <ctime>

namespace battery {

    double time();
    std::time_t epoch_time();
    std::time_t epoch_time_ms();
    std::time_t epoch_time_us();

    void sleep(double seconds);
    void sleep_ms(uint64_t milliseconds);
    void sleep_us(uint64_t microseconds);

    namespace internal {
        void reset_time();
        inline static std::time_t launch_time_us = 0;
    }

}
