#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class input : public b::widgets::base_widget {
    public:
        std::string content;
        std::string hint;
        int flags = ImGuiInputTextFlags_None;
        size_t buffer_size = 256;

        bool changed = false;
        bool active = false;

        input(py::object context = py::object()) : base_widget(context, "input") {}
        void operator()() override;

        inline static void define_python_types(py::module& module) {
            b::py::class_<b::widgets::input, b::widgets::base_widget>(module, "input")
                    .def(b::py::init<>())
                    .def(b::py::init<py::object>())
                    .def_readwrite("content", &b::widgets::input::content)
                    .def_readwrite("hint", &b::widgets::input::hint)
                    .def_readwrite("flags", &b::widgets::input::flags)
                    .def_readwrite("buffer_size", &b::widgets::input::buffer_size)
                    .def_readonly("changed", &b::widgets::input::changed)
                    .def_readonly("active", &b::widgets::input::active)
                    .def("__call__", [](b::widgets::input& self) { self(); });
        }

    private:
        std::string buffer;    // This is a buffer filled with zeros
    };

}