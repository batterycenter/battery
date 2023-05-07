#pragma once

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

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
