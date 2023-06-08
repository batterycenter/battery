#pragma once

#include "battery/graphics/widgets/base_widget.hpp"
#include "battery/graphics/widgets/container.hpp"

namespace b::widgets {

    class grid : public b::widgets::base_widget {
    public:
        bool frame_border = false;
        bool cell_border = false;
        std::vector<unit_property> cell_heights;
        std::vector<unit_property> cell_widths;

        widget_style children_style;

        grid(py::object context = py::object()) : base_widget(context, "grid") {}

        void operator()() override;
        void operator()(const std::function<void(std::function<void(int, int, std::function<void()>)>)>& callback);

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
            B_DEF_PY_WIDGET_SUBCLASS(grid, frame_border, cell_border, cell_heights, cell_widths, children_style)
            .def(py::init<>())
            .def(py::init<py::object>())
            .def("__call__", [](grid& self) { self(); })
            .def("__call__", [](grid& self, const std::function<void(std::function<void(int, int, std::function<void()>)>)>& callback) { self(callback); })
        )

    private:
        void calcWidths();
        void calcHeights();

    private:
        std::vector<float> widths;
        std::vector<float> heights;
        b::widgets::container grid_container;
        b::widgets::container worker_cell;
    };

}