#pragma once

#include "battery/graphics/widgets/base_widget.hpp"
#include "battery/graphics/canvas.hpp"

namespace b::widgets {

    class canvas_host : public b::widgets::base_widget {
    public:
        bool hovered = false;

        canvas_host(py::object context = py::object()) : base_widget(context, "canvas_host") {}

        void operator()() override;
        void operator()(const std::function<void(b::Canvas&)>& renderCallback);

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
                B_DEF_PY_WIDGET_SUBCLASS(canvas_host, hovered)
                        .def(py::init<>())
                        .def(py::init<py::object>())
                        .def("__call__", [](canvas_host& self) { self(); })
        )

    private:
        b::Canvas m_canvas;
    };

}
