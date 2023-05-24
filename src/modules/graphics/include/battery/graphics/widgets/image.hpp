#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class image : public b::widgets::base_widget {
    public:
        sf::Texture src;

        image(py::object context = py::object()) : base_widget(context, "image") {}
        void operator()() override;

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::image, b::widgets::base_widget>(module, "image")
                    .def(b::py::init<>())
                    .def(b::py::init<py::object>())
                    .def_readwrite("src", &b::widgets::image::src)
                    .def("__call__", [](b::widgets::image& self) { self(); });
        }
    };

}