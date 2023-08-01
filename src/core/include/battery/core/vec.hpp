#pragma once

#ifdef B_VEC_INTEGRATE_SFML
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"
#endif

#ifdef B_VEC_INTEGRATE_IMGUI
#include "imgui.h"
#endif

#ifdef B_VEC_INTEGRATE_GLM
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#endif

// You can set B_VEC_INTEGRATE_CUSTOM
// to your custom vector type conversion functions

#include "battery/core/log.hpp"

namespace b {

    // TODO: Define vec3 and vec4

    struct Vec2 {
        double x = 0.0;
        double y = 0.0;

        Vec2() = default;
        Vec2(double x, double y) : x(x), y(y) {}

        // TODO: Maybe clone SFML in terms of defining a constructor for polar coordinates?

        // TODO: Define constructors and getters for glm
        // TODO: Define constructors and getters for sf::Vectors of all types

        Vec2 operator+(const Vec2 &other) const { return {x + other.x, y + other.y}; }
        Vec2 operator-(const Vec2 &other) const { return {x - other.x, y - other.y}; }
        Vec2 operator*(const Vec2 &other) const { return {x * other.x, y * other.y}; }
        Vec2 operator/(const Vec2 &other) const { return {x / other.x, y / other.y}; }

        Vec2 operator+(double other) const { return {x + other, y + other}; }
        Vec2 operator-(double other) const { return {x - other, y - other}; }
        Vec2 operator*(double other) const { return {x * other, y * other}; }
        Vec2 operator/(double other) const { return {x / other, y / other}; }
        Vec2 operator-() const { return {-x, -y}; }

        Vec2& operator+=(const Vec2 &other) { x += other.x; y += other.y; return *this; }
        Vec2& operator-=(const Vec2 &other) { x -= other.x; y -= other.y; return *this; }
        Vec2& operator*=(const Vec2 &other) { x *= other.x; y *= other.y; return *this; }
        Vec2& operator/=(const Vec2 &other) { x /= other.x; y /= other.y; return *this; }

        Vec2& operator+=(double other) { x += other; y += other; return *this; }
        Vec2& operator-=(double other) { x -= other; y -= other; return *this; }
        Vec2& operator*=(double other) { x *= other; y *= other; return *this; }
        Vec2& operator/=(double other) { x /= other; y /= other; return *this; }

        bool operator==(const Vec2 &other) const { return x == other.x && y == other.y; }
        bool operator!=(const Vec2 &other) const { return x != other.x || y != other.y; }

        double length() const { return std::sqrt(x * x + y * y); }
        double lengthSquared() const { return x * x + y * y; }          // Enough for comparisons, faster than length()

        Vec2 normalized(bool returnZeroInsteadOfThrow = false) const {
            if (!returnZeroInsteadOfThrow && length() == 0) {
                throw std::runtime_error("Cannot normalize a b::vec2 with length 0");
            }
            return length() == 0 ? Vec2(0, 0) : *this / length();
        }
        Vec2& normalize(bool returnZeroInsteadOfThrow = false) {
            return *this = normalized(returnZeroInsteadOfThrow);
        }

        // TODO: Maybe clone SFML in terms of defining angle functions to get the angle to X or angle to another vector?

        double dot(const Vec2 &other) const { return x * other.x + y * other.y; }
        double cross(const Vec2 &other) const { return x * other.y - y * other.x; }

        Vec2 lerp(const Vec2 &other, double t) const { return *this + (other - *this) * t; }

        Vec2 projected(const Vec2 &other) const { return other * (dot(other) / other.lengthSquared()); }
        Vec2 reflected(const Vec2 &normal) const { return *this - normal * (2.0 * dot(normal)); }

        Vec2 rotated(double angle) const {
            return {x * std::cos(angle) - y * std::sin(angle), x * std::sin(angle) + y * std::cos(angle)};
        }

        Vec2& rotate(double angle) { return *this = rotated(angle); }

        Vec2 perpendicular() const { return {-y, x}; }
        Vec2& makePerpendicular() { return *this = perpendicular(); }

#ifdef B_VEC_INTEGRATE_SFML
        Vec2(const sf::Vector2f &other) : x(other.x), y(other.y) {}
        Vec2(const sf::Vector2i &other) : x(other.x), y(other.y) {}
        Vec2(const sf::Vector2u &other) : x(other.x), y(other.y) {}
        Vec2& operator=(const sf::Vector2f &other) { x = other.x; y = other.y; return *this; }
        Vec2& operator=(const sf::Vector2i &other) { x = other.x; y = other.y; return *this; }
        Vec2& operator=(const sf::Vector2u &other) { x = other.x; y = other.y; return *this; }
        operator sf::Vector2f() const { return {static_cast<float>(x), static_cast<float>(y)}; }
        operator sf::Vector2i() const { return {static_cast<int>(x), static_cast<int>(y)}; }
        operator sf::Vector2u() const { return {static_cast<unsigned int>(x), static_cast<unsigned int>(y)}; }
#endif

#ifdef B_VEC_INTEGRATE_IMGUI
        Vec2(const ImVec2 &other) : x(other.x), y(other.y) {}
        Vec2& operator=(const ImVec2 &other) { x = other.x; y = other.y; return *this; }
        operator ImVec2() const { return {static_cast<float>(x), static_cast<float>(y)}; }
#endif

#ifdef B_VEC_INTEGRATE_GLM
        vec2(const glm::vec2 &other) : x(other.x), y(other.y) {}
        vec2(const glm::ivec2 &other) : x(other.x), y(other.y) {}
        vec2(const glm::uvec2 &other) : x(other.x), y(other.y) {}
        vec2& operator=(const glm::vec2 &other) { x = other.x; y = other.y; return *this; }
        vec2& operator=(const glm::ivec2 &other) { x = other.x; y = other.y; return *this; }
        vec2& operator=(const glm::uvec2 &other) { x = other.x; y = other.y; return *this; }
        operator glm::vec2() const { return {static_cast<float>(x), static_cast<float>(y)}; }
        operator glm::ivec2() const { return {static_cast<int>(x), static_cast<int>(y)}; }
        operator glm::uvec2() const { return {static_cast<unsigned int>(x), static_cast<unsigned int>(y)}; }
#endif

#ifdef B_VEC_INTEGRATE_CUSTOM
        B_VEC_INTEGRATE_CUSTOM  // This is a macro that can be defined by the user to integrate with their own vector types
#endif
    };

} // namespace b

template <> struct fmt::formatter<b::Vec2> {
    constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator {
        return ctx.begin();
    }
    auto format(const b::Vec2& vec, format_context& ctx) const -> format_context::iterator {
        return fmt::format_to(ctx.out(), "[{:.6f}, {:.6f}]", vec.x, vec.y);
    }
};