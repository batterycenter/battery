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

        ImVec2 actual_position;
        ImVec2 actual_size;
        bool titlebar_hovered = false;
        bool is_open = true;

        window() : base_widget("Battery Window") {}
        explicit window(int flags) : base_widget("Battery Window"), flags(flags) {}
        explicit window(const std::string& name) : base_widget(name) {}
        explicit window(const std::string& name, int flags) : base_widget(name), flags(flags) {}

        void operator()() override {}
        void operator()(const std::function<void()>& callback);

        inline static void define_python_class(py::module& module) {
            b::py::class_<b::widgets::window>(module, "window")
                .def(b::py::init<>())
                .def(b::py::init<int>())
                .def(b::py::init<const std::string&>())
                .def(b::py::init<const std::string&, int>())
                .def_readwrite("flags", &b::widgets::window::flags)
                .def_readwrite("border_width", &b::widgets::window::border_width)
                .def_readwrite("position", &b::widgets::window::position)
                .def_readwrite("size", &b::widgets::window::size)
                .def_readwrite("always_open", &b::widgets::window::always_open)
                .def_readwrite("style", &b::widgets::window::style)
                .def_readwrite("children_style", &b::widgets::window::children_style)
                .def_readwrite("actual_position", &b::widgets::window::actual_position)
                .def_readwrite("actual_size", &b::widgets::window::actual_size)
                .def_readwrite("titlebar_hovered", &b::widgets::window::titlebar_hovered)
                .def_readwrite("is_open", &b::widgets::window::is_open)
                .def("__call__", [](b::widgets::window& self) { self(); });
        }
    };

}