
#include "battery/core/random.hpp"
#include <random>

namespace b {

    double random(double min, double max) {
        static std::random_device dev;
        static std::mt19937 rng(dev());
        std::uniform_real_distribution<double> dist(min, max);
        return dist(rng);
    }

    int randomInt(int min, int max) {
        static std::random_device dev;
        static std::mt19937 rng(dev());
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

}