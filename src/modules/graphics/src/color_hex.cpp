
#include "battery/graphics/color_hex.hpp"
#include "battery/log.hpp"

namespace b {

    ImColor color_hex(const b::string& hex) {
        if (hex.length() != 7 && hex.length() != 9) {    // Parse #RRGGBB or #RRGGBBAA format
            throw std::invalid_argument(b::format("Cannot parse '{}' as an HTML color code: Unexpected length", hex));
        }

        ImColor color;
        color.Value.x = static_cast<double>(std::stoi(hex.substr(1, 2), nullptr, 16));
        color.Value.y = static_cast<double>(std::stoi(hex.substr(3, 2), nullptr, 16));
        color.Value.z = static_cast<double>(std::stoi(hex.substr(5, 2), nullptr, 16));
        color.Value.w = hex.size() == 9 ? static_cast<double>(std::stoi(hex.substr(7, 2), nullptr, 16)) : 255;
        return color;
    }

    b::string color_hex(ImColor color) {
        return b::format("#{:02X}{:02X}{:02X}{:02X}",
                         static_cast<int>(color.Value.x),
                         static_cast<int>(color.Value.y),
                         static_cast<int>(color.Value.z),
                         static_cast<int>(color.Value.w));
    }

}