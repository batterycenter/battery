#pragma once

#include <vector>
#include <string>

namespace battery {

	const char** args_to_cstr(const std::vector<std::string>& args);

	// This function is to be defined by the user (outside the Battery library)
    int battery_main(const std::vector<std::string>& args);

    // This function is only to be called when BATTERY_CORE_NO_MAIN is defined
    int run_main(int argc, const char** argv);

}
