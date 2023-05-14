
#include "battery/graphics/sfml.hpp"

namespace b {

    void define_imgui_python_types(b::py::module& module) {
        py::class_<ImVec2>(module, "ImVec2")
            .def(b::py::init<>())
            .def(b::py::init<float, float>())
            .def_readwrite("x", &ImVec2::x)
            .def_readwrite("y", &ImVec2::y)
            .def("__repr__", [](const ImVec2& v) {
                return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
            })
            .def("__setattr__", [](ImVec2& self, b::py::tuple t) {
                if (t.size() != 2) {
                    throw std::runtime_error("Invalid tuple size for ImVec2 initialization");
                }
                self.x = t[0].cast<float>();
                self.y = t[1].cast<float>();
            })
            .def("__getattr__", [](const ImVec2& self) {
                return b::py::make_tuple(self.x, self.y);
            });

        py::enum_<ImGuiWindowFlags_>(module, "ImGuiWindowFlags", py::arithmetic())
            .value("NoTitleBar", ImGuiWindowFlags_NoTitleBar)
            .value("NoResize", ImGuiWindowFlags_NoResize)
            .value("NoMove", ImGuiWindowFlags_NoMove)
            .value("NoScrollbar", ImGuiWindowFlags_NoScrollbar)
            .value("NoScrollWithMouse", ImGuiWindowFlags_NoScrollWithMouse)
            .value("NoCollapse", ImGuiWindowFlags_NoCollapse)
            .value("AlwaysAutoResize", ImGuiWindowFlags_AlwaysAutoResize)
            .value("NoBackground", ImGuiWindowFlags_NoBackground)
            .value("NoSavedSettings", ImGuiWindowFlags_NoSavedSettings)
            .value("NoMouseInputs", ImGuiWindowFlags_NoMouseInputs)
            .value("MenuBar", ImGuiWindowFlags_MenuBar)
            .value("HorizontalScrollbar", ImGuiWindowFlags_HorizontalScrollbar)
            .value("NoFocusOnAppearing", ImGuiWindowFlags_NoFocusOnAppearing)
            .value("NoBringToFrontOnFocus", ImGuiWindowFlags_NoBringToFrontOnFocus)
            .value("AlwaysVerticalScrollbar", ImGuiWindowFlags_AlwaysVerticalScrollbar)
            .value("AlwaysHorizontalScrollbar", ImGuiWindowFlags_AlwaysHorizontalScrollbar)
            .value("AlwaysUseWindowPadding", ImGuiWindowFlags_AlwaysUseWindowPadding)
            .value("NoNavInputs", ImGuiWindowFlags_NoNavInputs)
            .value("NoNavFocus", ImGuiWindowFlags_NoNavFocus)
            .value("UnsavedDocument", ImGuiWindowFlags_UnsavedDocument)
            .value("NoDocking", ImGuiWindowFlags_NoDocking)
            .value("NoNav", ImGuiWindowFlags_NoNav)
            .value("NoDecoration", ImGuiWindowFlags_NoDecoration)
            .value("NoInputs", ImGuiWindowFlags_NoInputs);
    }

}