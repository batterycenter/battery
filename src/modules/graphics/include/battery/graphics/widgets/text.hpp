#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class text : public b::widgets::base_widget {
    public:

        std::string label = "Label";
        widget_style style;

        bool sameline = false;

        text() : base_widget("Text") {}
        explicit text(const std::string& label) : base_widget("Text"), label(label) {}

        void operator()() override;

        inline static void define_python_class(py::module& module) {
            b::py::class_<b::widgets::text>(module, "text")
                    .def(b::py::init<>())
                    .def(b::py::init<const std::string&>())
                    .def_readwrite("label", &b::widgets::text::label)
                    .def_readwrite("style", &b::widgets::text::style)
                    .def_readwrite("sameline", &b::widgets::text::sameline)
                    .def("__call__", [](b::widgets::text& self) { self(); });
        }

    };

}