#pragma once

#include "Battery/common.h"

namespace Battery {

    using Args_t = std::vector<std::string>;

	const char** ArgsToCStr(const Args_t& _args);

	// This function is to be defined by the user (outside of Battery)
    int Main(const Args_t& args);

}
