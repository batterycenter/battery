#pragma once

#include <cstdint>

namespace battery {

    void sleep(double seconds);
    void sleep_ms(uint64_t milliseconds);
    void sleep_us(uint64_t microseconds);

}
