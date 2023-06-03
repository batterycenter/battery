#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class input : public b::widgets::base_widget {
    public:
        b::string content;
        b::string hint;
        int flags = ImGuiInputTextFlags_None;
        size_t buffer_size = 256;

        bool changed = false;
        bool active = false;

        input(py::object context = py::object()) : base_widget(context, "input") {}
        void operator()() override;

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
            B_DEF_PY_WIDGET_SUBCLASS(input, content, hint, flags, buffer_size, changed, active)
            .def(py::init<>())
            .def(py::init<py::object>())
            .def("__call__", [](input& self) { self(); })
        )

    private:
        b::string buffer;    // This is a buffer filled with zeros
    };

}