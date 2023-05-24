
#include "battery/graphics/unit_property.hpp"
#include "battery/graphics/sfml.hpp"
#include "battery/graphics/types.hpp"
#include "battery/graphics/font_stack.hpp"

namespace b {

    void define_python_types(b::py::module& module) {
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
            .value("None", ImGuiWindowFlags_None)
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

        py::enum_<ImDrawFlags_>(module, "ImDrawFlags", py::arithmetic())
            .value("None", ImDrawFlags_None)
            .value("Closed", ImDrawFlags_Closed)
            .value("RoundCornersTopLeft", ImDrawFlags_RoundCornersTopLeft)
            .value("RoundCornersTopRight", ImDrawFlags_RoundCornersTopRight)
            .value("RoundCornersBottomLeft", ImDrawFlags_RoundCornersBottomLeft)
            .value("RoundCornersBottomRight", ImDrawFlags_RoundCornersBottomRight)
            .value("RoundCornersNone", ImDrawFlags_RoundCornersNone)
            .value("RoundCornersTop", ImDrawFlags_RoundCornersTop)
            .value("RoundCornersBottom", ImDrawFlags_RoundCornersBottom)
            .value("RoundCornersLeft", ImDrawFlags_RoundCornersLeft)
            .value("RoundCornersRight", ImDrawFlags_RoundCornersRight)
            .value("RoundCornersAll", ImDrawFlags_RoundCornersAll)
            .value("RoundCornersDefault_", ImDrawFlags_RoundCornersDefault_)
            .value("RoundCornersMask_", ImDrawFlags_RoundCornersMask_);

        py::enum_<ImGuiInputTextFlags_>(module, "ImGuiInputTextFlags", py::arithmetic())
            .value("None", ImGuiInputTextFlags_None)
            .value("CharsDecimal", ImGuiInputTextFlags_CharsDecimal)
            .value("CharsHexadecimal", ImGuiInputTextFlags_CharsHexadecimal)
            .value("CharsUppercase", ImGuiInputTextFlags_CharsUppercase)
            .value("CharsNoBlank", ImGuiInputTextFlags_CharsNoBlank)
            .value("AutoSelectAll", ImGuiInputTextFlags_AutoSelectAll)
            .value("EnterReturnsTrue", ImGuiInputTextFlags_EnterReturnsTrue)
            .value("CallbackCompletion", ImGuiInputTextFlags_CallbackCompletion)
            .value("CallbackHistory", ImGuiInputTextFlags_CallbackHistory)
            .value("CallbackAlways", ImGuiInputTextFlags_CallbackAlways)
            .value("CallbackCharFilter", ImGuiInputTextFlags_CallbackCharFilter)
            .value("AllowTabInput", ImGuiInputTextFlags_AllowTabInput)
            .value("CtrlEnterForNewLine", ImGuiInputTextFlags_CtrlEnterForNewLine)
            .value("NoHorizontalScroll", ImGuiInputTextFlags_NoHorizontalScroll)
            .value("AlwaysInsertMode", ImGuiInputTextFlags_AlwaysOverwrite)
            .value("ReadOnly", ImGuiInputTextFlags_ReadOnly)
            .value("Password", ImGuiInputTextFlags_Password)
            .value("NoUndoRedo", ImGuiInputTextFlags_NoUndoRedo)
            .value("CharsScientific", ImGuiInputTextFlags_CharsScientific)
            .value("CallbackResize", ImGuiInputTextFlags_CallbackResize)
            .value("Multiline", ImGuiInputTextFlags_CallbackEdit)
            .value("NoMarkEdited", ImGuiInputTextFlags_EscapeClearsAll);

        py::enum_<b::alignh>(module, "AlignH", py::arithmetic())
            .value("Left", b::alignh::LEFT)
            .value("Center", b::alignh::CENTER)
            .value("Right", b::alignh::RIGHT);

        py::enum_<b::alignv>(module, "AlignV", py::arithmetic())
            .value("Top", b::alignv::TOP)
            .value("Center", b::alignv::CENTER)
            .value("Bottom", b::alignv::BOTTOM);

        py::class_<sf::Texture>(module, "sfTexture");
        py::class_<ImFont>(module, "ImFont");
        b::unit_property::define_python_types(module);

        module.def("sameline", &ImGui::SameLine);
        module.def("sameline", []() { ImGui::SameLine(); });
        module.def("load_font", &b::load_font, py::return_value_policy::reference);
        module.def("push_font", &b::push_font);
        module.def("pop_font", &b::pop_font);
        module.def("show_demo_window", &ImGui::ShowDemoWindow);
        module.def("show_demo_window", []() { ImGui::ShowDemoWindow(nullptr); });
    }

}