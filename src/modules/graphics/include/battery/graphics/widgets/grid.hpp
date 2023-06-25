#pragma once

#include "battery/graphics/widgets/base_widget.hpp"
#include "battery/graphics/widgets/container.hpp"

namespace b::widgets {

    class grid : public b::widgets::base_widget {
    public:
        bool frameBorder = false;
        bool cellBorder = false;
        std::vector<unit_property> cellHeights;
        std::vector<unit_property> cellWidths;

        widget_style childrenStyle;

        grid(py::object context = py::object()) : base_widget(context, "grid") {}

        void operator()() override;
        void operator()(const std::function<void(std::function<void(int, int, std::function<void()>)>)>& callback);

        B_DEF_PY_WIDGET_CONTEXT_FUNC(
            B_DEF_PY_WIDGET_SUBCLASS(grid, frameBorder, cellBorder, cellHeights, cellWidths, childrenStyle)
            .def(py::init<>())
            .def(py::init<py::object>())
            .def("__call__", [](grid& self) { self(); })
            .def("__call__", [](grid& self, const std::function<void(std::function<void(int, int, std::function<void()>)>)>& callback) { self(callback); })
        )

    private:
        void calcWidths();
        void calcHeights();

    private:
        std::vector<double> m_widths;
        std::vector<double> m_heights;
        b::widgets::container m_gridContainer;
        b::widgets::container m_workerCell;
    };

}