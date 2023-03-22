#pragma once

#include "battery/core/all.hpp"
#include "Error.h"

class ProjectGenerator {
public:
    ProjectGenerator() {}

    void writeToDisk();
    [[nodiscard]] b::expected<std::nullopt_t, Error> run();
};
