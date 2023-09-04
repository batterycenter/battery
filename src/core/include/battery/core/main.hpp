
#ifndef BATTERY_CORE_MAIN_HPP
#define BATTERY_CORE_MAIN_HPP

#include <string>
#include <vector>

#include "battery/core/string.hpp"

namespace b {

    // This function is to be defined by the user (outside the Battery library)
    extern int main(const std::vector<std::string>& args);

	const char** args_to_argv(const std::vector<std::string>& args);

    int invoke_main(int argc, const char** argv);

}

#endif // BATTERY_CORE_MAIN_HPP
