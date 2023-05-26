#pragma once

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"
#include "SFML/Audio.hpp"
#include "battery/python/python.hpp"
#include "battery/core/string.hpp"

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

inline bool operator==(const ImVec2& lhs, const ImVec2& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
inline bool operator!=(const ImVec2& lhs, const ImVec2& rhs) { return !(lhs == rhs); }

inline bool operator==(const ImVec4& lhs, const ImVec4& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w; }
inline bool operator!=(const ImVec4& lhs, const ImVec4& rhs) { return !(lhs == rhs); }

namespace pybind11::detail {
    template <> struct type_caster<b::string> : public type_caster_base<b::string> {
        using base = type_caster_base<b::string>;
    public:
        bool load(handle src, bool convert) {
            if (base::load(src, convert)) {
                return true;
            }
            else if (b::py::isinstance<b::py::str>(src)) {
                value = new b::string(b::py::cast<std::string>(src));
                return true;
            }
            return false;
        }

        static handle cast(const b::string& src, return_value_policy policy, handle parent) {
            return b::py::cast(std::string(src), policy, parent).release();
        }
    };
}

namespace b {
    void define_python_types(b::py::module& module);

    float distance(const ImVec2& p1, const ImVec2& p2);
    float distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

    float degrees(float radians);
    float radians(float degrees);

    float length(const ImVec2& v);
    float length(const sf::Vector2f& v);

    float dot(const ImVec2& v1, const ImVec2& v2);
    float dot(const sf::Vector2f& v1, const sf::Vector2f& v2);

    ImVec2 round(const ImVec2& v);
    sf::Vector2f round(const sf::Vector2f& v);

    ImVec2 min(const ImVec2& v1, const ImVec2& v2);
    sf::Vector2f min(const sf::Vector2f& v1, const sf::Vector2f& v2);
    ImVec2 max(const ImVec2& v1, const ImVec2& v2);
    sf::Vector2f max(const sf::Vector2f& v1, const sf::Vector2f& v2);

    float map(float value, float min1, float max1, float min2, float max2);
    ImVec2 map(const ImVec2& value, const ImVec2& min1, const ImVec2& max1, const ImVec2& min2, const ImVec2& max2);
    sf::Vector2f map(const sf::Vector2f& value, const sf::Vector2f& min1, const sf::Vector2f& max1, const sf::Vector2f& min2, const sf::Vector2f& max2);
}
