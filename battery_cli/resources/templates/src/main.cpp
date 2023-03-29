
#include "battery/core/all.hpp"

int b::main(const std::vector<std::string>& args) {

    b::print(b::print_color::GREEN, "Hello World!");

    b::print("Args: ");
    for (const auto& arg : args) {
        b::print(" - {}", arg);
    }

    return 0;
}