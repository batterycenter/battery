#pragma once

#include <vector>
#include <string>

#include "battery/core/string.hpp"  // std::u8string printable

namespace b {

    // This function is to be defined by the user (outside the Battery library)
    extern int main(const std::vector<std::u8string>& args);

	const char** args_to_argv(const std::vector<std::u8string>& args);

    // This function is only to be called manually when BATTERY_CORE_NO_MAIN is defined
    int invoke_main(int argc, const char** argv);

}
