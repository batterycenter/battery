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

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::grid, b::widgets::base_widget>(module, "grid")
                .def(b::py::init<>())
                .def(b::py::init<py::object>())
                .def_readwrite("frame_border", &b::widgets::grid::frame_border)
                .def_readwrite("cell_border", &b::widgets::grid::cell_border)
                .def_readwrite("cell_heights", &b::widgets::grid::cell_heights)
                .def_readwrite("cell_widths", &b::widgets::grid::cell_widths)
                .def_readwrite("children_style", &b::widgets::grid::children_style)
                .def("__call__", [](b::widgets::grid& self) { self(); })
                .def("__call__", [](b::widgets::grid& self, const std::function<void(std::function<void(int, int, std::function<void()>)>)>& callback) { self(callback); });
        }

    private:
        void calc_widths();
        void calc_heights();

    private:
        std::vector<float> widths;
        std::vector<float> heights;
        b::widgets::container grid_container;
        b::widgets::container worker_cell;
    };

}