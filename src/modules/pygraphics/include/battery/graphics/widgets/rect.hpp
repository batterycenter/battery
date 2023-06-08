#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class rect : public b::widgets::base_widget {
    public:
        int flags = ImDrawFlags_None;
        unit_property fill_color = "#FFFFFF"_u;

        rect(py::object context = py::object()) : base_widget(context, "rect") {}
        void operator()() override;

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
            B_DEF_PY_WIDGET_SUBCLASS(rect, flags, fill_color)
            .def(py::init<>())
            .def(py::init<py::object>())
            .def("__call__", [](rect& self) { self(); })
        )
    };

}