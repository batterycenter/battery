#pragma once

#include "battery/graphics/widgets/base_widget.hpp"
#include "battery/graphics/widgets/checkbox.hpp"

namespace b::widgets {

    class checkbox_ex : public b::widgets::checkbox {
    public:
        float inner_padding_factor = 1.0f;

        checkbox_ex(py::object context = py::object()) : checkbox(context) {}

        void operator()() override;

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::checkbox_ex, b::widgets::checkbox>(module, "checkbox_ex")
                .def(b::py::init<>())
                .def(b::py::init<py::object>())
                .def_readwrite("inner_padding_factor", &b::widgets::checkbox_ex::inner_padding_factor)
                .def("__call__", &b::widgets::checkbox::operator());
        }

    private:
        bool Checkbox(const char* label, bool* v);
    };

}