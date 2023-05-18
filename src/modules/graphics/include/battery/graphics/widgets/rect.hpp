#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class rect : public b::widgets::base_widget {
    public:
        int flags = ImDrawFlags_None;
        unit_property fill_color = "#FFFFFF"_u;

        rect(py::object context = py::object()) : base_widget(context, "rect") {}
        void operator()() override;

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::rect, b::widgets::base_widget>(module, "rect")
            .def(b::py::init<>())
                    .def(b::py::init<py::object>())
                    .def_readwrite("flags", &b::widgets::rect::flags)
                    .def_readwrite("fill_color", &b::widgets::rect::fill_color)
                    .def("__call__", &b::widgets::rect::operator());
        }
    };

}