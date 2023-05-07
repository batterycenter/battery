#pragma once

#include "battery/core/string.hpp"
#include "battery/graphics/sfml.hpp"

namespace b {

    class unit_property {
    public:

        enum class unit {
            NONE,               // Invalid
            UNITLESS,           // eg. '2.7'                        -> (string)
            PIXEL,              // eg. '35.2px'                     -> (string)
            PERCENT,            // eg. '17%'                        -> (string)
            VEC2,               // eg. { 45, 28.7 } or ImVec2(3, 4) -> (json/object)
            COLOR_HEX           // eg. '#FF0000' or '#FF0000FF'     -> (string)
        };

        unit_property() = default;
        unit_property(const std::string& value);
        unit_property(float value, unit unit);
        unit_property(float value);
        unit_property(ImVec2 value);
        unit_property(ImVec4 color);

        unit_property& operator=(const std::string& value);
        unit_property& operator=(float value);
        unit_property& operator=(ImVec2 value);
        unit_property& operator=(ImVec4 color);

        explicit operator float() const;
        explicit operator ImVec2() const;
        explicit operator ImVec4() const;
        explicit operator std::string() const;

        [[nodiscard]] unit unit() const { return m_unit; }

        bool operator==(const unit_property& other) { return str() == other.str(); }
        bool operator!=(const unit_property& other) { return !this->operator==(other); }

        [[nodiscard]] std::string str() const;

    private:
        void parse_numeric_property(const std::string& str);
        void parse_color(const std::string& str);
        void clear();

        float m_float = 0.f;
        ImVec2 m_vec2 {};
        ImVec4 m_vec4 {};
        enum unit m_unit = unit::NONE;
    };

//    inline std::ostream& operator<<(std::ostream& os, const PropertyValue& value) {
//        os << value.str();
//        return os;
//    }

}
