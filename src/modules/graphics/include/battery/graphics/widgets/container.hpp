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

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::container, b::widgets::base_widget>(module, "container")
                .def(b::py::init<>())
                .def(b::py::init<py::object>())
                .def_readwrite("flags", &b::widgets::container::flags)
                .def_readwrite("native_window_border", &b::widgets::container::native_window_border)
                .def_readwrite("children_style", &b::widgets::container::children_style)
                .def("__call__", [](b::widgets::container& self) { self(); })
                .def("__call__", [](b::widgets::container& self, const std::function<void()>& callback) { self(callback); });
        }

    private:
        void calc_cell_sizes();

    private:
        std::vector<float> column_widths;
        std::vector<float> row_heights;
    };

}