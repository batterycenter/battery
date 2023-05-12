#pragma once

#include "battery/graphics/widgets/base_widget.hpp"

namespace b::widgets {

    class input : public b::widgets::base_widget {
    public:

        std::string content;
        std::string hint;
        int flags = ImGuiInputTextFlags_None;
        widget_style style;
        bool sameline = false;
        size_t buffer_size = 256;

        bool changed = false;
        bool active = false;
        ImVec2 actual_size;

        input() : base_widget("Input") {}
        explicit input(const std::string& name) : base_widget(name) {}

        void operator()() override;

        inline static void define_python_class(py::module& module) {
            b::py::class_<b::widgets::input>(module, "input")
                    .def(b::py::init<>())
                    .def(b::py::init<const std::string&>())
                    .def_readwrite("content", &b::widgets::input::content)
                    .def_readwrite("hint", &b::widgets::input::hint)
                    .def_readwrite("flags", &b::widgets::input::flags)
                    .def_readwrite("style", &b::widgets::input::style)
                    .def_readwrite("sameline", &b::widgets::input::sameline)
                    .def_readonly("changed", &b::widgets::input::changed)
                    .def_readonly("active", &b::widgets::input::active)
                    .def_readonly("actual_size", &b::widgets::input::actual_size)
                    .def("__call__", [](b::widgets::input& self) { self(); });
        }

    private:
        std::string buffer;    // This is a buffer filled with zeros
    };

}