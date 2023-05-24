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

inline ImVec2 operator-(const ImVec2& self) {
    return ImVec2(-self.x, -self.y);
}

inline ImVec4 operator+(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs); }
inline ImVec4 operator-(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs); }
inline ImVec4 operator*(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); }
inline ImVec4 operator/(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs); }

inline ImVec4 operator+=(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs); }
inline ImVec4 operator-=(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs); }
inline ImVec4 operator*=(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); }
inline ImVec4 operator/=(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs); }

inline bool operator==(const ImVec4& lhs, const ImVec4& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

inline bool operator!=(const ImVec4& lhs, const ImVec4& rhs) {
    return !(lhs == rhs);
}

namespace b {
    void define_python_types(b::py::module& module);

    float distance(const ImVec2& p1, const ImVec2& p2);
    float distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

    float degrees(float radians);
    float radians(float degrees);

    float length(const ImVec2& v);
    float length(const sf::Vector2f& v);

    float map(float value, float min1, float max1, float min2, float max2);
    ImVec2 map(const ImVec2& value, const ImVec2& min1, const ImVec2& max1, const ImVec2& min2, const ImVec2& max2);
    sf::Vector2f map(const sf::Vector2f& value, const sf::Vector2f& min1, const sf::Vector2f& max1, const sf::Vector2f& min2, const sf::Vector2f& max2);
}
