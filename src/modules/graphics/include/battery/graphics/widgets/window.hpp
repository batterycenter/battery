#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class window : public base_widget {
    public:
        int flags = ImGuiWindowFlags_None;
        std::optional<float> border_width;

        std::optional<ImVec2> position;
        std::optional<ImVec2> size;
        bool always_open = false;
        widget_style style;
        widget_style children_style;

        bool titlebar_hovered = false;
        bool is_open = true;

        window(py::object context = py::object()) : base_widget(std::move(context), "battery window") {}

        void operator()() override { this->operator()({}); }
        void operator()(const std::function<void()>& callback);

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::window, b::widgets::base_widget>(module, "window")
                    .def(b::py::init<>())
                    .def(b::py::init<py::object>())
                    .def_readwrite("flags", &b::widgets::window::flags)
                    .def_readwrite("border_width", &b::widgets::window::border_width)
                    .def_readwrite("position", &b::widgets::window::position)
                    .def_readwrite("size", &b::widgets::window::size)
                    .def_readwrite("always_open", &b::widgets::window::always_open)
                    .def_readwrite("style", &b::widgets::window::style)
                    .def_readwrite("children_style", &b::widgets::window::children_style)
                    .def_readwrite("titlebar_hovered", &b::widgets::window::titlebar_hovered)
                    .def_readwrite("is_open", &b::widgets::window::is_open)
                    .def("__call__", [](b::widgets::window& self) { self({}); })
                    .def("__call__", [](b::widgets::window& self, const std::function<void()>& callback) { self(callback); });
        }
    };

}