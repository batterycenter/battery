#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class text : public b::widgets::base_widget {
    public:

        std::string label = "Label";
        widget_style style;

        bool sameline = false;

        text(py::object context = py::object()) : base_widget(context, "##text") {}

        void operator()() override;

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::text, b::widgets::base_widget>(module, "text")
                    .def(b::py::init<>())
                    .def(b::py::init<py::object>())
                    .def_readwrite("label", &b::widgets::text::label)
                    .def_readwrite("style", &b::widgets::text::style)
                    .def_readwrite("sameline", &b::widgets::text::sameline)
                    .def("__call__", [](b::widgets::text& self) { self(); });
        }

    };

}