#pragma once

#include "battery/core/string.hpp"

namespace b {

    double random(double min, double max);

    inline double random(double max) {
        return random(0.0, max);
    }

    int randomInt(int min, int max);

    inline int randomInt(int max) {
        return randomInt(0, max);
    }

    inline bool randomBool() {
        return randomInt(0, 1) == 1;
    }

}