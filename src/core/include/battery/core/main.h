#pragma once

#include <vector>
#include <string>

namespace battery {

    using Args_t = std::vector<std::string>;

	const char** args_to_cstr(const Args_t& _args);

	// This function is to be defined by the user (outside of Battery)
    int main(const Args_t& args);

}
