#pragma once

#include "battery/graphics/widgets/button.hpp"

namespace b::widgets {

    class fancy_button : public b::widgets::button {
    public:
        fancy_button(py::object context = py::object());

        void operator()();  // NOT OVERRIDE, we need both the base class operator and this one

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::fancy_button, b::widgets::button>(module, "fancy_button")
                    .def(b::py::init<>())
                    .def(b::py::init<py::object>())
                    .def("__call__", &b::widgets::button::operator());
        }
    };

}
