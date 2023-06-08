#pragma once

#include "battery/graphics/widgets/button.hpp"

namespace b::widgets {

    class button_ex : public b::widgets::button {
    public:
        button_ex(py::object context = py::object());

        void operator()();  // NOT OVERRIDE, we need both the base class operator and this one

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
            b::py::class_<button_ex, button>(module, "button_ex")
            .def(py::init<>())
            .def(py::init<py::object>())
            .def("__call__", [](button_ex& self) { self(); })
        )
    };

}
