
#include "battery/core/all.hpp"

int b::main(const std::vector<b::string>& args) {

    b::print(b::colors::fg::green, "Hello World!\n");

    b::print("Args: \n");
    for (const auto& arg : args) {
        b::print(" - {}\n", arg);
    }

    return 0;
}