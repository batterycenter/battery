#pragma once

#include <vector>
#include <string>

#include "battery/core/string.hpp"

namespace b {

    // This function is to be defined by the user (outside the Battery library)
    extern int main(const std::vector<b::string>& args);

	const char** args_to_argv(const std::vector<b::string>& args);

    int invoke_main(int argc, const char** argv);

}