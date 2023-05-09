
#include "battery/graphics/unit_property.hpp"
#include "battery/graphics/color_hex.hpp"

namespace b {

    unit_property::unit_property(const std::string& value) { operator=(value); }
    unit_property::unit_property(float value) { operator=(value); }
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

    unit_property& unit_property::operator=(ImVec4 color) {
        clear();
        m_vec4 = color;
        m_unit = unit::COLOR_HEX;
        return *this;
    }

    unit_property::operator float() const {
        return numeric();
    }

    unit_property::operator ImVec4() const {
        return color();
    }

    unit_property::operator std::string() const {
        return string();
    }

    float unit_property::numeric() const {
        if (m_unit == unit::UNITLESS || m_unit == unit::PERCENT || m_unit == unit::PIXEL || m_unit == unit::EM) {
            return m_float;
        }
        else {
            throw std::runtime_error("Cannot convert b::unit_property to float: property hold a non-float value!");
        }
    }

    ImVec4 unit_property::color() const {
        if (m_unit == unit::COLOR_HEX) {
            return m_vec4;
        }
        else {
            throw std::runtime_error("Cannot convert b::unit_property to ImVec4/Color: property hold a non-color value!");
        }
    }

    std::string unit_property::string() const {
        switch (m_unit) {
            case unit::UNITLESS: return fmt::format("{:.06}", m_float);
            case unit::PERCENT: return fmt::format("{:.06}%", m_float);
            case unit::PIXEL: return fmt::format("{:.06}px", m_float);
            case unit::COLOR_HEX: return color_hex(m_vec4);
            case unit::EM: return fmt::format("{:.06}em", m_float);
            default: return {};
        }
    }

    void unit_property::parse_numeric_property(const std::string& str) {
        float tempValue {};
        enum unit tempUnit {};

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
            if (unit == "%") tempUnit = unit::PERCENT;
            else if (unit == "px") tempUnit = unit::PIXEL;
            else if (unit == "em") tempUnit = unit::EM;
            else throw std::invalid_argument(fmt::format("Cannot load value '{}' into b::unit_property: Unexpected unit!", str));
        }

        m_float = tempValue;
        m_unit = tempUnit;
    }

    void unit_property::parse_color(const std::string& str) {
        m_unit = unit::NONE;
        m_vec4 = color_hex(str);    // Throws if invalid
        m_unit = unit::COLOR_HEX;
    }

    void unit_property::clear() {
        m_float = 0.f;
        m_vec4 = ImVec4(0.f, 0.f, 0.f, 0.f);
        m_unit = unit::NONE;
    }
}