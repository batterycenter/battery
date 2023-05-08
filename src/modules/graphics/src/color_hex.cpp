
#include "battery/graphics/color_hex.hpp"
#include "battery/core/log.hpp"

namespace b {

    ImVec4 color_hex(const std::string& hex) {
        if (hex.length() != 7 && hex.length() != 9) {    // Parse #RRGGBB or #RRGGBBAA format
            throw std::invalid_argument(fmt::format("Cannot parse '{}' as an HTML color code: Unexpected length", hex));
        }

        std::stringstream ss;
        ImVec4 color;
        color.x = (float)std::stoi(hex.substr(1, 2), nullptr, 16);
        color.y = (float)std::stoi(hex.substr(3, 2), nullptr, 16);
        color.z = (float)std::stoi(hex.substr(5, 2), nullptr, 16);
        color.w = hex.size() == 9 ? (float)std::stoi(hex.substr(7, 2), nullptr, 16) : 255;
        return color;
    }

    std::string color_hex(ImVec4 color) {
        return fmt::format("#{:02X}{:02X}{:02X}{:02X}", (int)color.x, (int)color.y, (int)color.z, (int)color.w);
    }

}