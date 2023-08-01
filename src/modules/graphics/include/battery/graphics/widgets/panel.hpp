#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class panel : public base_widget {
    public:
        bool alwaysOpen = false;
        widget_style childrenStyle;

        struct Flags {
            bool noResize = true;
            bool noMove = true;
            bool noCollapse = true;
            bool noTitleBar = true;
            bool noScrollbar = false;
            bool noBringToFrontOnFocus = false;
            bool noNavFocus = false;
            bool noBackground = false;
            B_DEF_PY_STATIC_CONTEXT_FUNC(
                B_DEF_PY_RAW_CLASS(Flags,
                                   noResize, noMove, noCollapse, noTitleBar,
                                   noScrollbar, noBringToFrontOnFocus, noNavFocus, noBackground)
            )
        } flags;
        ImGuiWindowFlags rawFlags = ImGuiWindowFlags_None;

        bool titlebarHovered = false;
        bool isOpen = true;

        panel(py::object context = py::object()) : base_widget(std::move(context), "battery panel") {}

        void operator()() override { this->operator()({}); }
        void operator()(const std::function<void()>& callback);

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
                Flags::definePythonClass(module);
                B_DEF_PY_WIDGET_SUBCLASS(panel, alwaysOpen, childrenStyle, flags, rawFlags, titlebarHovered, isOpen)
                    .def(py::init<>())
                    .def(py::init<py::object>())
                    .def("__call__", [](panel& self) { self({}); })
                    .def("__call__", [](panel& self, const std::function<void()>& callback) { self(callback); })
        )
    };

}