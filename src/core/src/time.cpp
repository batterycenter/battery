
#include "battery/core/time.hpp"

#include <thread>
#include <chrono>

namespace b {

    double time() {
        if (internal::launch_time_us == 0) {
            internal::reset_time();
        }
        return static_cast<double>(epoch_time_us() - internal::launch_time_us) / 1000000.0;
    }

    std::time_t epoch_time() {
        using namespace std::chrono;
        return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
    }

    std::time_t epoch_time_ms() {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    std::time_t epoch_time_us() {
        using namespace std::chrono;
        return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
    }

    void sleep(double seconds) {
        sleep_us(static_cast<uint64_t>(seconds * 1000000.0));
    }

    void sleep_ms(uint64_t milliseconds) {
        sleep_us(milliseconds * 1000);
    }

    void sleep_us(uint64_t microseconds) {
        std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
    }

    namespace internal {
        void reset_time() {
            internal::launch_time_us = epoch_time_us();
        }
        std::time_t launch_time_us = 0;
    }
}
