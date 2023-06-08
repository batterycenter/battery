#pragma once

#include "battery/graphics/widgets/base_widget.hpp"
#include "battery/graphics/widgets/checkbox.hpp"

namespace b::widgets {

    class checkbox_ex : public b::widgets::checkbox {
    public:
        float inner_padding_factor = 1.0f;

        checkbox_ex(py::object context = py::object()) : checkbox(context) {}

        void operator()() override;

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
            B_DEF_PY_WIDGET_SUBCLASS(checkbox_ex, inner_padding_factor)
            .def(py::init<>())
            .def(py::init<py::object>())
            .def("__call__", [](checkbox_ex& self) { self(); })
        )

    private:
        bool Checkbox(const char* label, bool* v);
    };

}