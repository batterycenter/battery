#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class panel : public base_widget {
    public:
        int flags = ImGuiWindowFlags_None;

        bool always_open = false;
        widget_style children_style;

        bool titlebar_hovered = false;
        bool is_open = true;

        panel(py::object context = py::object()) : base_widget(std::move(context), "battery panel") {}

        void operator()() override { this->operator()({}); }
        void operator()(const std::function<void()>& callback);

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
            B_DEF_PY_WIDGET_SUBCLASS(panel, flags, always_open, children_style, titlebar_hovered, is_open)
            .def("__call__", [](panel& self) { self({}); })
            .def("__call__", [](panel& self, const std::function<void()>& callback) { self(callback); })
        )
    };

}