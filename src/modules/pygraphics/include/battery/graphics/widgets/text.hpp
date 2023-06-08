#pragma once

#include "battery/graphics/widgets/base_widget.hpp"
#include "battery/graphics/widgets/container.hpp"

namespace b::widgets {

    class text : public b::widgets::base_widget {
    public:
        b::string label = "Label";
        AlignH alignh = AlignH::Left;
        AlignV alignv = AlignV::Center;
        bool underline = false;
        float underline_offset = 2.0f;

        bool hyperlink = false;
        bool hyperlink_hovered = false;
        bool hyperlink_clicked = false;

        text(py::object context = py::object()) : base_widget(context, "##text") {}
        void operator()() override;

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
            B_DEF_PY_WIDGET_SUBCLASS(text, label, alignh, alignv, underline, underline_offset, hyperlink, hyperlink_hovered, hyperlink_clicked)
            .def(py::init<>())
            .def(py::init<py::object>())
            .def("__call__", [](text& self) { self(); })
        )

    private:
        b::widgets::container text_container;
    };

}