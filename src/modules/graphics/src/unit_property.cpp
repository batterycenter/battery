
#include "battery/graphics/unit_property.hpp"

namespace b {

    unit_property::unit_property(const std::string& value) { operator=(value); }
    unit_property::unit_property(float value) { operator=(value); }
    unit_property::unit_property(ImVec2 value) { operator=(value); }
    unit_property::unit_property(ImVec4 color) { operator=(color); }

    unit_property::unit_property(float value, enum unit unit) {
        this->m_unit = unit;
        operator=(value);
    }

    unit_property& unit_property::operator=(const std::string& value) {
        clear();
        if (value.empty()) throw std::invalid_argument("Cannot load value: Value string is empty!");

        if (value[0] == '#') {
            parse_color(value);
        }
        else {
            parse_numeric_property(value);
        }
        return *this;
    }

    unit_property& unit_property::operator=(float value) {
        clear();
        m_float = value;
        return *this;
    }

    unit_property& unit_property::operator=(ImVec2 value) {
        clear();
        m_vec2 = value;
        m_unit = unit::VEC2;
        return *this;
    }

    unit_property& unit_property::operator=(ImVec4 color) {
        clear();
        m_vec4 = color;
        m_unit = unit::COLOR_HEX;
        return *this;
    }

    unit_property::operator float() const {
        if (m_unit == unit::UNITLESS || m_unit == unit::PERCENT || m_unit == unit::PIXEL) {
            return m_float;
        }
        else {
            throw std::runtime_error("Cannot convert b::unit_property to float: property hold a non-float value!");
        }
    }

    unit_property::operator ImVec2() const {
        if (m_unit == unit::VEC2) {
            return m_vec2;
        }
        else {
            throw std::runtime_error("Cannot convert b::unit_property to ImVec2: property hold a non-ImVec2 value!");
        }
    }

    unit_property::operator ImVec4() const {
        if (m_unit == unit::COLOR_HEX) {
            return m_vec4;
        }
        else {
            throw std::runtime_error("Cannot convert b::unit_property to ImVec4/Color: property hold a non-color value!");
        }
    }

    unit_property::operator std::string() const {
        return this->str();
    }

    std::string unit_property::str() const {
        std::stringstream str;
        switch (m_unit) {
            case unit::UNITLESS: return fmt::format("{:.06}", m_float);
            case unit::PERCENT: return fmt::format("{:.06}%", m_float);
            case unit::PIXEL: return fmt::format("{:.06}px", m_float);
            case unit::VEC2: return fmt::format("[{:.06}, {:.06}]", m_vec2.x, m_vec2.y);
            case unit::COLOR_HEX: return fmt::format(fmt::runtime("#{:02X}{:02X}{:02X}{:02X}"), m_vec4.x, m_vec4.y, m_vec4.z, m_vec4.w);
            default: return {};
        }
    }

    void unit_property::parse_numeric_property(const std::string& str) {
        float tempValue = 0.f;
        enum unit tempUnit = unit::NONE;

        if (std::string("0123456789.-").find(str[0]) == std::string::npos)       // Doesn't start with a digit
            throw std::invalid_argument(fmt::format("Cannot load value '{}' into b::unit_property: Expected a digit!", str));

        size_t numberDigits = str.find_first_not_of("0123456789.-");                  // Scan how many numeric characters we have
        if (numberDigits == std::string::npos) numberDigits = str.length();            // It must consist only of digits

        try {
            tempValue = std::stof(str.substr(0, numberDigits));
        }
        catch (...) {
            throw std::invalid_argument(fmt::format("Cannot load value '{}' into b::unit_property: Parsing as float failed", str));
        }

        if (numberDigits == str.length()) {     // Only numeric characters
            tempUnit = unit::UNITLESS;
        }
        else {
            std::string unit = str.substr(numberDigits, str.length() - numberDigits);
            switch(unit[0]) {
                case '%': tempUnit = unit::PERCENT; break;
                case 'p': tempUnit = unit::PIXEL; break;
                default: throw std::invalid_argument(fmt::format("Cannot load value '{}' into b::unit_property: Unexpected unit!", str));
            }
        }

        m_float = tempValue;
        m_unit = tempUnit;
    }

    void unit_property::parse_color(const std::string& str) {
        std::stringstream ss;
        if (str.length() == 7 || str.length() == 9) {    // Parse #RRGGBB or #RRGGBBAA format
            m_vec4.x = (float)std::stoi(str.substr(1, 2), nullptr, 16);
            m_vec4.y = (float)std::stoi(str.substr(3, 2), nullptr, 16);
            m_vec4.z = (float)std::stoi(str.substr(5, 2), nullptr, 16);
            m_vec4.w = str.size() == 9 ? (float)std::stoi(str.substr(7, 2), nullptr, 16) : 255;
            m_unit = unit::COLOR_HEX;
        }
        else {
            m_unit = unit::NONE;
            throw std::invalid_argument(fmt::format("Cannot load value '{}' as an HTML color code: Unexpected length!", str));
        }
    }

    void unit_property::clear() {
        m_float = 0.f;
        m_vec2 = ImVec2(0.f, 0.f);
        m_vec4 = ImVec4(0.f, 0.f, 0.f, 0.f);
        m_unit = unit::NONE;
    }
}