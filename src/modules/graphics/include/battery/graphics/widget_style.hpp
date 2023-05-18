#pragma once

#include "battery/python/python.hpp"
#include "battery/graphics/sfml.hpp"
#include "battery/graphics/unit_property.hpp"
#include "battery/graphics/property_stack.hpp"
#include "battery/graphics/font_stack.hpp"

namespace b {

    class widget_style {
    public:
        std::optional<std::string> font;

        widget_style() = default;

        inline property_pack& operator[](const std::string& str) {
            return properties[str];
        }

        inline void push() {
            pushed_properties = properties;
            for (auto& [key, property] : pushed_properties) {
                property_stack::push(key, property);
            }
            if (font.has_value()) {
                b::push_font(font.value());
            }
        }

        inline void pop() {
            for (auto& [key, property] : pushed_properties) {
                property_stack::pop();
            }
            if (font.has_value()) {
                b::pop_font();
            }
        }

        inline property_pack& get(const std::string& str) {
            return properties[str];
        }

        inline void set(const std::string& str, const std::string& pack) {
            properties[str] = pack;
        }

        inline void set(const std::string& str, const std::pair<std::string, std::string>& pack) {
            properties[str] = { pack.first, pack.second };
        }

        inline static void define_python_types(b::py::module& module) {
            b::py::class_<b::widget_style>(module, "widget_style")
                    .def(b::py::init<>())
                    .def_readwrite("font", &b::widget_style::font)
                    .def("__getitem__", &b::widget_style::get)
                    .def("__setitem__", static_cast<void (widget_style::*)(const std::string&, const std::string&)>(&b::widget_style::set))
                    .def("__setitem__", static_cast<void (widget_style::*)(const std::string&, const std::pair<std::string, std::string>&)>(&b::widget_style::set))
                    .def("push", &b::widget_style::push)
                    .def("pop", &b::widget_style::pop);
        }

    private:
        std::unordered_map<std::string, property_pack> properties;
        std::unordered_map<std::string, property_pack> pushed_properties;
    };

}