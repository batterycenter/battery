#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class image : public b::widgets::base_widget {
    public:

        widget_style style;
        sf::Texture img;
        ImVec2 size;
        ImVec2 actual_size;
        bool sameline = false;

        image() : base_widget("Image") {}
        explicit image(const std::string& name) : base_widget(name) {}

        void operator()() override;

        inline static void define_python_class(py::module& module) {
            b::py::class_<b::widgets::image>(module, "image")
                    .def(b::py::init<>())
                    .def(b::py::init<const std::string&>())
                    .def_readwrite("style", &b::widgets::image::style)
                    .def_readwrite("size", &b::widgets::image::size)
                    .def_readwrite("actual_size", &b::widgets::image::actual_size)
                    .def_readwrite("sameline", &b::widgets::image::sameline)
                    .def("__call__", [](b::widgets::image& self) { self(); });
        }
    };

}