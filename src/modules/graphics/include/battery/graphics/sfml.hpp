#pragma once

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"
#include "SFML/Audio.hpp"
#include "battery/python/python.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui-SFML.h"

inline ImVec4 operator+(const ImVec4& lhs, float rhs) {
    return ImVec4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
}

inline ImVec4 operator-(const ImVec4& lhs, float rhs) {
    return ImVec4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
}

inline ImVec4 operator*(const ImVec4& lhs, float rhs) {
    return ImVec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

inline ImVec4 operator/(const ImVec4& lhs, float rhs) {
    return ImVec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}

inline bool operator==(const ImVec4& lhs, const ImVec4& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

inline bool operator!=(const ImVec4& lhs, const ImVec4& rhs) {
    return !(lhs == rhs);
}

namespace b {

    inline void define_imgui_python_types(b::py::module& module) {
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

    }

}
