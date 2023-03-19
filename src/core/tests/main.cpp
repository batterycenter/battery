
// This file only defines an empty main function, so battery can initialize itself
#include "battery/core/all.hpp"
#include <gtest/gtest.h>

int b::main(const std::vector<std::string>& args) {
    int argc = args.size();
    auto argv = b::args_to_argv(args);
    ::testing::InitGoogleTest(&argc, const_cast<char**>(argv));
    return RUN_ALL_TESTS();
}