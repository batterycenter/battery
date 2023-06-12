#pragma once

#include "battery/core/string.hpp"

namespace b {

    float random(float min, float max);

    inline float random(float max) {
        return random(0.0f, max);
    }

    int randomInt(int min, int max);

    inline int randomInt(int max) {
        return randomInt(0, max);
    }

    inline bool randomBool() {
        return randomInt(0, 1) == 1;
    }

}