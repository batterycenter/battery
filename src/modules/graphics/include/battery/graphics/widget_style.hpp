#pragma once

#include "battery/python/python.hpp"
#include "battery/graphics/sfml.hpp"
#include "battery/graphics/unit_property.hpp"
#include "battery/graphics/property_stack.hpp"
#include "battery/graphics/font_stack.hpp"
#include "battery/graphics/context.hpp"

namespace b {

    class widget_style {
    public:
        std::optional<b::string> font;

        widget_style() = default;

        inline property_pack& operator[](const b::string& str) {
            return properties[str];
        }

        inline void push() {
            pushed_properties = properties;
            for (auto& [key, property] : pushed_properties) {
                property_stack::push(key, property);
            }
            if (font.has_value()) {
                b::PushFont(font.value());
            }
        }

        inline void pop() {
            for (auto& [key, property] : pushed_properties) {
                property_stack::pop();
            }
            if (font.has_value()) {
                b::PopFont();
            }
        }

        inline property_pack& get(const b::string& str) {
            return properties[str];
        }

        inline void set(const b::string& str, const b::string& pack) {
            properties[str] = pack;
        }

        inline void set(const b::string& str, const std::pair<b::string, b::string>& pack) {
            properties[str] = { pack.first, pack.second };
        }

        B_DEF_PY_STATIC_CONTEXT_FUNC(
            B_DEF_PY_RAW_CLASS(widget_style, font)
            .def("__getitem__", static_cast<property_pack& (widget_style::*)(const b::string&)>(&b::widget_style::get))
            .def("__setitem__", static_cast<void (widget_style::*)(const b::string&, const b::string&)>(&b::widget_style::set))
            .def("__setitem__", static_cast<void (widget_style::*)(const b::string&, const std::pair<b::string, b::string>&)>(&b::widget_style::set))
            .def("push", &b::widget_style::push)
            .def("pop", &b::widget_style::pop)
        )

    private:
        std::unordered_map<b::string, property_pack> properties;
        std::unordered_map<b::string, property_pack> pushed_properties;
    };

}