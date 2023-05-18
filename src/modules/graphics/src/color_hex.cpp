
#include "battery/graphics/color_hex.hpp"
#include "battery/core/log.hpp"

namespace b {

    ImColor color_hex(const std::string& hex) {
        if (hex.length() != 7 && hex.length() != 9) {    // Parse #RRGGBB or #RRGGBBAA format
            throw std::invalid_argument(fmt::format("Cannot parse '{}' as an HTML color code: Unexpected length", hex));
        }

        std::stringstream ss;
        ImColor color;
        color.Value.x = (float)std::stoi(hex.substr(1, 2), nullptr, 16);
        color.Value.y = (float)std::stoi(hex.substr(3, 2), nullptr, 16);
        color.Value.z = (float)std::stoi(hex.substr(5, 2), nullptr, 16);
        color.Value.w = hex.size() == 9 ? (float)std::stoi(hex.substr(7, 2), nullptr, 16) : 255;
        return color;
    }

    std::string color_hex(ImColor color) {
        return fmt::format("#{:02X}{:02X}{:02X}{:02X}", (int)color.Value.x, (int)color.Value.y, (int)color.Value.z, (int)color.Value.w);
    }

}