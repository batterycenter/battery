#pragma once

#include "pch.hpp"
#include "resources/version_txt.h"
using namespace Battery;

#include "tools.h"
#include "config.h"

enum class Result {
    SUCCESS = 0,

    // Positive error codes: Printed to the terminal
    INTERNAL_ERROR = 1,
    PROJECT_FILE_NOT_FOUND = 2,

    // Negative error codes: Recorded and returned silently, but no console print (print already happened elsewhere)
    CLI_INVALID_ARGUMENTS = -1,
    PROJECT_FILE_INVALID = -2,
};

using Err = std::pair<Result, std::string>;

Err cli_new();
Err cli_configure();
Err cli_build();
Err cli_run();

/*template<typename T>
std::string rangToString(T color) {
    rang::setControlMode(rang::control::Force);
    rang::setWinTermMode(rang::winTerm::Ansi);

    std::stringstream ss;
    ss << color;

    rang::setControlMode(rang::control::Auto);
    rang::setWinTermMode(rang::winTerm::Auto);

    return ss.str();
}*/
