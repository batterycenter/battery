#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class container : public b::widgets::base_widget {
    public:
        int flags = ImGuiWindowFlags_None;
        bool native_window_border = false;

        widget_style children_style;

        container(py::object context = py::object()) : base_widget(context, "container") {}

        void operator()() override;
        void operator()(const std::function<void()>& callback);

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
            B_DEF_PY_WIDGET_SUBCLASS(container, flags, native_window_border, children_style)
            .def(py::init<>())
            .def(py::init<py::object>())
            .def("__call__", [](container& self) { self(); })
            .def("__call__", [](container& self, const std::function<void()>& callback) { self(callback); })
        )

    private:
        void calc_cell_sizes();

    private:
        std::vector<double> column_widths;
        std::vector<double> row_heights;
    };

}