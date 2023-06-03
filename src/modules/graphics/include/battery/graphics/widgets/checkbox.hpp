#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class checkbox : public b::widgets::base_widget {
    public:
        bool state = false;

        checkbox(py::object context = py::object()) : base_widget(context, "checkbox") {}
        void operator()() override;

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
            B_DEF_PY_WIDGET_SUBCLASS(checkbox, state)
            .def(py::init<>())
            .def(py::init<py::object>())
            .def("__call__", [](checkbox& self) { self(); })
        )
    };

}