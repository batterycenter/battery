
#include "battery/graphics/unit_property.hpp"
#include "battery/graphics/color_hex.hpp"
#include "battery/log.hpp"

namespace b {

    unit_property::unit_property(const b::string& value) { operator=(value); }
    unit_property::unit_property(double value) { operator=(value); }
    unit_property::unit_property(ImColor color) { operator=(color); }
    unit_property::unit_property(sf::Color color) { operator=(color); }

    unit_property::unit_property(double value, enum Unit unit) {
        m_numeric = value;
        m_unit = unit;
    }

    unit_property& unit_property::operator=(const b::string& value) {
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

    unit_property& unit_property::operator=(double value) {
        clear();
        m_numeric = value;
        m_unit = Unit::UNITLESS;
        return *this;
    }

    unit_property& unit_property::operator=(ImColor color) {
        clear();
        m_color = color;
        m_unit = Unit::COLOR_HEX;
        return *this;
    }

    unit_property& unit_property::operator=(sf::Color color) {
        clear();
        m_color = color.toInteger();
        m_unit = Unit::COLOR_HEX;
        return *this;
    }

    unit_property::operator double() const {
        return numeric();
    }

    unit_property::operator ImColor() const {
        return color();
    }

    unit_property::operator b::string() const {
        return string();
    }

    double unit_property::numeric() const {
        if (m_unit == Unit::UNITLESS || m_unit == Unit::PERCENT || m_unit == Unit::PIXEL || m_unit == Unit::EM) {
            return m_numeric;
        }
        else {
            throw std::runtime_error("Cannot convert b::unit_property to numeric: property holds a non-numeric value!");
        }
    }

    ImColor unit_property::color() const {
        if (m_unit == Unit::COLOR_HEX) {
            return m_color;
        }
        else {
            throw std::runtime_error("Cannot convert b::unit_property to ImColor: property holds a non-color value!");
        }
    }

    b::string unit_property::string() const {
        switch (m_unit) {
            case Unit::UNITLESS: return b::format("{:.06}", m_numeric);
            case Unit::PERCENT: return b::format("{:.06}%", m_numeric);
            case Unit::PIXEL: return b::format("{:.06}px", m_numeric);
            case Unit::COLOR_HEX: return color_hex(m_color);
            case Unit::EM: return b::format("{:.06}em", m_numeric);
            default: return {};
        }
    }

    void unit_property::parse_numeric_property(const b::string& str) {
        double tempValue {};
        enum Unit tempUnit {};

        if (b::string("0123456789.-").find(str[0]) == b::string::npos)       // Doesn't start with a digit
            throw std::invalid_argument(b::format("Cannot load value '{}' into b::unit_property: Expected a digit!", str));

        size_t numberDigits = str.find_first_not_of("0123456789.-");                  // Scan how many numeric characters we have
        if (numberDigits == b::string::npos) numberDigits = str.length();            // It must consist only of digits

        try {
            tempValue = std::stof(str.substr(0, numberDigits));
        }
        catch (...) {
            throw std::invalid_argument(b::format("Cannot load value '{}' into b::unit_property: Parsing as numeric failed", str));
        }

        if (numberDigits == str.length()) {     // Only numeric characters
            tempUnit = Unit::UNITLESS;
        }
        else {
            b::string unit = str.substr(numberDigits, str.length() - numberDigits);
            if (unit == "%") tempUnit = Unit::PERCENT;
            else if (unit == "px") tempUnit = Unit::PIXEL;
            else if (unit == "em") tempUnit = Unit::EM;
            else throw std::invalid_argument(b::format("Cannot load value '{}' into b::unit_property: Unexpected unit!", str));
        }

        m_numeric = tempValue;
        m_unit = tempUnit;
    }

    void unit_property::parse_color(const b::string& str) {
        m_unit = Unit::NONE;
        m_color = color_hex(str);    // Throws if invalid
        m_unit = Unit::COLOR_HEX;
    }

    void unit_property::clear() {
        m_numeric = 0.f;
        m_color = ImColor(0.f, 0.f, 0.f, 0.f);
        m_unit = Unit::NONE;
    }
}