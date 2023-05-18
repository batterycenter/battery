#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class checkbox : public b::widgets::base_widget {
    public:
        bool state = false;

        checkbox(py::object context = py::object()) : base_widget(context, "checkbox") {}
        void operator()() override;

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::checkbox, b::widgets::base_widget>(module, "checkbox")
                .def(b::py::init<>())
                .def(b::py::init<py::object>())
                .def_readwrite("state", &b::widgets::checkbox::state)
                .def("__call__", &b::widgets::checkbox::operator());
        }
    };

}