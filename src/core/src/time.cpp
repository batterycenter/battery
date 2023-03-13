
#include "battery/core/time.h"

#include <thread>
#include <chrono>

namespace battery {

    void sleep(double seconds) {
        sleep_us(static_cast<uint64_t>(seconds * 1000000.0));
    }

    void sleep_ms(uint64_t milliseconds) {
        sleep_us(milliseconds * 1000);
    }

    void sleep_us(uint64_t microseconds) {
        std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
    }

}
