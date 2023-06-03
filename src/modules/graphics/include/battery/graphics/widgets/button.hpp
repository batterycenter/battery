#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class button : public b::widgets::base_widget {
    public:

        bool clicked = false;		// Single trigger when clicked
        bool held = false;
        bool hovered = false;

        button(py::object context = py::object()) : base_widget(context, "button") {}

        void operator()() override;

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
            B_DEF_PY_WIDGET_SUBCLASS(button, clicked, held, hovered)
            .def(py::init<>())
            .def(py::init<py::object>())
            .def("__call__", [](button& self) { self(); })
        )

    protected:
        std::function<std::tuple<bool, bool, bool>()> custom_implementation {};
    };

}
