#pragma once

#include "battery/python/python.hpp"
#include "battery/graphics/sfml.hpp"
#include "battery/graphics/unit_property.hpp"
#include "battery/graphics/property_stack.hpp"

namespace b {

    class widget_style {
    public:
        widget_style() = default;

        inline property_pack& operator[](const std::string& str) {
            return properties[str];
        }

        inline void push() {
            if (pushed) throw std::logic_error("Cannot push b::widgets::style property: Push/Pop mismatch!");
            pushed_properties = properties;
            for (auto& [key, property] : pushed_properties) {
                property_stack::push(key, property);
            }
            pushed = true;
        }

        inline void pop() {
            if (!pushed) throw std::logic_error("Cannot pop b::widgets::style property: Push/Pop mismatch!");
            for (auto& [key, property] : pushed_properties) {
                property_stack::pop();
            }
            pushed = false;
        }

        inline property_pack& get(const std::string& str) {
            return properties[str];
        }

        inline void set(const std::string& str, const std::string& pack) {
            properties[str] = pack;
        }

        inline static void define_python_types(b::py::module& module) {
            b::py::class_<b::widget_style>(module, "widget_style")
                    .def(b::py::init<>())
                    .def("__getitem__", &b::widget_style::get)
                    .def("__setitem__", &b::widget_style::set)
                    .def("push", &b::widget_style::push)
                    .def("pop", &b::widget_style::pop);
        }

    private:
        bool pushed = false;
        std::unordered_map<std::string, property_pack> properties;
        std::unordered_map<std::string, property_pack> pushed_properties;
    };

}