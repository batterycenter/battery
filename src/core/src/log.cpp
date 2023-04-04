
#include "battery/core/log.hpp"
#include <iostream>

#define RANG_IMPLEMENTATION                         // This file defines the implementation: By doing so, it separates
#include "battery/core/internal/extern/rang.hpp"    // <iostream>, the rang library and in turn the Windows API from the header
                                                    // Thus, this cannot be defined in the header or we'd be
namespace b {                                       // polluting the global namespace with the Windows API and iostream

    void print(const std::string& str) {
        std::cout << str;
    }

    void print(const b_color_variants_t& color, const std::string& str) {
        std::visit([&str](auto&& color) { std::cout << color << str << rang::fg::reset; }, color);
    }

    void print(const std::vector<b_color_variants_t>& colors, const std::string& str) {
        for (auto& any_color : colors) {
            b::print(any_color, str);
        }
    }

}
