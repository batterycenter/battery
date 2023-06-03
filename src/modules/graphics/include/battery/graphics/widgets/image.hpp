#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class image : public b::widgets::base_widget {
    public:
        sf::Texture src;

        image(py::object context = py::object()) : base_widget(context, "image") {}
        void operator()() override;

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
            B_DEF_PY_WIDGET_SUBCLASS(image, src)
            .def(py::init<>())
            .def(py::init<py::object>())
            .def("__call__", [](image& self) { self(); })
        )
    };

}