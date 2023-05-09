#pragma once

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"
#include "SFML/Audio.hpp"

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
