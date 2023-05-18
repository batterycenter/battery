#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class button : public b::widgets::base_widget {
    public:

        bool clicked = false;		// Single trigger when clicked
        bool held = false;
        bool hovered = false;

        button(py::object context = py::object()) : base_widget(context, "button") {}

        void operator()() override;

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::button, b::widgets::base_widget>(module, "button")
                .def(b::py::init<>())
                .def(b::py::init<py::object>())
                .def_readwrite("clicked", &b::widgets::button::clicked)
                .def_readwrite("held", &b::widgets::button::held)
                .def_readwrite("hovered", &b::widgets::button::hovered)
                .def("__call__", &b::widgets::button::operator());
        }

    protected:
        std::function<std::tuple<bool, bool, bool>()> custom_implementation {};
    };

}
