#pragma once

#include "battery/graphics/widgets/base_widget.hpp"
#include "battery/graphics/widgets/container.hpp"

namespace b::widgets {

    class text : public b::widgets::base_widget {
    public:
        std::string label = "Label";
        alignh alignh = alignh::LEFT;
        alignv alignv = alignv::CENTER;
        bool underline = false;
        float underline_offset = 2.0f;

        bool hyperlink = false;
        bool hyperlink_hovered = false;
        bool hyperlink_clicked = false;

        text(py::object context = py::object()) : base_widget(context, "##text") {}
        void operator()() override;

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::text, b::widgets::base_widget>(module, "text")
                    .def(b::py::init<>())
                    .def(b::py::init<py::object>())
                    .def_readwrite("label", &b::widgets::text::label)
                    .def_readwrite("alignh", &b::widgets::text::alignh)
                    .def_readwrite("alignv", &b::widgets::text::alignv)
                    .def_readwrite("underline", &b::widgets::text::underline)
                    .def_readwrite("underline_offset", &b::widgets::text::underline_offset)
                    .def_readwrite("hyperlink", &b::widgets::text::hyperlink)
                    .def_readwrite("hyperlink_clicked", &b::widgets::text::hyperlink_clicked)
                    .def("__call__", [](b::widgets::text& self) { self(); });
        }

    private:
        b::widgets::container text_container;
    };

}