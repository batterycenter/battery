#pragma once

#include "battery/core/string.hpp"
#include "battery/graphics/sfml.hpp"
#include "battery/python/python.hpp"

namespace b {

    enum class unit {                   // BE CAREFUL WHEN CHANGING!!! (implementations check == COLOR_HEX as synonym for NOT NUMERIC)
        NONE,               // Invalid
        UNITLESS,           // eg. '2.7'
        PIXEL,              // eg. '35.2px'
        PERCENT,            // eg. '17%'
        COLOR_HEX,          // eg. '#FF0000' or '#FF0000FF'
        EM                  // eg. '2em'
    };

    class unit_property {
    public:

        unit_property() = default;
        unit_property(const std::string& value);
        unit_property(float value, unit unit);
        unit_property(float value);
        unit_property(ImColor color);
        unit_property(sf::Color color);

        unit_property& operator=(const std::string& value);
        unit_property& operator=(float value);
        unit_property& operator=(ImColor color);
        unit_property& operator=(sf::Color color);

        explicit operator float() const;
        explicit operator ImColor() const;
        explicit operator std::string() const;

        [[nodiscard]] float numeric() const;
        [[nodiscard]] ImColor color() const;
        [[nodiscard]] std::string string() const;

        [[nodiscard]] unit unit() const { return m_unit; }
        [[nodiscard]] bool defined() const { return m_unit != b::unit::NONE; }

        bool operator==(const unit_property& other) { return this->string() == other.string(); }
        bool operator!=(const unit_property& other) { return !this->operator==(other); }

        inline static void define_python_types(b::py::module& module) {

            b::py::enum_<b::unit>(module, "unit")
                .value("NONE", b::unit::NONE)
                .value("UNITLESS", b::unit::UNITLESS)
                .value("PIXEL", b::unit::PIXEL)
                .value("PERCENT", b::unit::PERCENT)
                .value("COLOR_HEX", b::unit::COLOR_HEX)
                .value("EM", b::unit::EM)
                .export_values();

            b::py::class_<b::unit_property>(module, "unit_property")
                .def(b::py::init<>())
                .def(b::py::init<const std::string&>())
                .def(b::py::init<float, b::unit>())
                .def(b::py::init<float>())
                .def(b::py::init<ImColor>())
                .def("string", &b::unit_property::string)
                .def("numeric", &b::unit_property::numeric)
                .def("color", &b::unit_property::color)
                .def("unit", &b::unit_property::unit)
                .def("set", [](b::unit_property& self, const std::string& value) { self = value; })
                .def("set", [](b::unit_property& self, float value) { self = value; })
                .def("set", [](b::unit_property& self, ImColor value) { self = value; })
                .def("__eq__", &b::unit_property::operator==)
                .def("__ne__", &b::unit_property::operator!=);
        }

    private:
        void parse_numeric_property(const std::string& str);
        void parse_color(const std::string& str);
        void clear();

        float m_float = 0.f;
        ImColor m_color {};
        enum unit m_unit = unit::NONE;
    };

}

inline std::ostream& operator<<(std::ostream& os, const b::unit_property& value) {
    os << value.string();
    return os;
}

inline b::unit_property operator "" _u(const char* str, std::size_t len) {
    return b::unit_property(std::string(str, len));
}
