#pragma once

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"
#include "SFML/Audio.hpp"
#include "battery/core/string.hpp"
#include "battery/core/extern/json.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "battery/core/vec.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

inline ImVec4 operator+(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs); }
inline ImVec4 operator-(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs); }
inline ImVec4 operator*(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); }
inline ImVec4 operator/(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs); }

inline ImVec4 operator+=(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs); }
inline ImVec4 operator-=(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs); }
inline ImVec4 operator*=(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); }
inline ImVec4 operator/=(const ImVec4& lhs, float rhs) { return ImVec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs); }

inline bool operator==(const ImVec4& lhs, const ImVec4& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w; }
inline bool operator!=(const ImVec4& lhs, const ImVec4& rhs) { return !(lhs == rhs); }

namespace nlohmann {
    template<> struct adl_serializer<sf::Color> {
        static void to_json(json& j, const sf::Color& col) {
            j = { col.r, col.g, col.b, col.a };
        }
        static void from_json(const json& j, sf::Color& opt) {
            opt.r = j[0];
            opt.g = j[1];
            opt.b = j[2];
            opt.a = j[3];
        }
    };
    template<> struct adl_serializer<sf::Vector2f> {
        static void to_json(json& j, const sf::Vector2f& vec) {
            j = { vec.x, vec.y };
        }
        static void from_json(const json& j, sf::Vector2f& opt) {
            opt.x = j[0];
            opt.y = j[1];
        }
    };
    template<> struct adl_serializer<sf::Vector2i> {
        static void to_json(json& j, const sf::Vector2i& vec) {
            j = { vec.x, vec.y };
        }
        static void from_json(const json& j, sf::Vector2i& opt) {
            opt.x = j[0];
            opt.y = j[1];
        }
    };
    template<> struct adl_serializer<sf::Vector2u> {
        static void to_json(json& j, const sf::Vector2u& vec) {
            j = { vec.x, vec.y };
        }
        static void from_json(const json& j, sf::Vector2u& opt) {
            opt.x = j[0];
            opt.y = j[1];
        }
    };
    template<> struct adl_serializer<ImVec2> {
        static void to_json(json& j, const ImVec2& vec) {
            j = { vec.x, vec.y };
        }
        static void from_json(const json& j, ImVec2& opt) {
            opt.x = j[0];
            opt.y = j[1];
        }
    };
    template<> struct adl_serializer<ImVec4> {
        static void to_json(json& j, const ImVec4& vec) {
            j = { vec.x, vec.y, vec.z, vec.w };
        }
        static void from_json(const json& j, ImVec4& opt) {
            opt.x = j[0];
            opt.y = j[1];
            opt.z = j[2];
            opt.w = j[3];
        }
    };
}

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

    double distance(const b::Vec2& p1, const b::Vec2& p2);

    double degrees(double radians);
    double radians(double degrees);

    b::Vec2 round(const b::Vec2& v);    // TODO: Maybe this should go in b::vec2 too?

    b::Vec2 min(const b::Vec2& v1, const b::Vec2& v2);
    b::Vec2 max(const b::Vec2& v1, const b::Vec2& v2);

    // TODO: Refactor lerp functions to be more consistent with all possible usages (Arduino/Processing/ImGui/...)

    double lerp(double val, double val_min, double val_max, double new_min, double new_max);
    b::Vec2 lerp(const b::Vec2& val, const b::Vec2& val_min, const b::Vec2& val_max, const b::Vec2& new_min, const b::Vec2& new_max);
}
