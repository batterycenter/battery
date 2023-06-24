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

namespace b {

    // TODO: Define vec3 and vec4

    struct vec2 {
        double x = 0.0;
        double y = 0.0;

        vec2() = default;
        vec2(double x, double y) : x(x), y(y) {}

#ifdef B_VEC_INTEGRATE_SFML
        vec2(const sf::Vector2f &other) : x(other.x), y(other.y) {}
        vec2(const sf::Vector2i &other) : x(other.x), y(other.y) {}
        vec2(const sf::Vector2u &other) : x(other.x), y(other.y) {}
#endif

#ifdef B_VEC_INTEGRATE_GLM
        vec2(const glm::vec2 &other) : x(other.x), y(other.y) {}
        vec2(const glm::ivec2 &other) : x(other.x), y(other.y) {}
        vec2(const glm::uvec2 &other) : x(other.x), y(other.y) {}
#endif

        // TODO: Maybe clone SFML in terms of defining a constructor for polar coordinates?

        // TODO: Define constructors and getters for glm
        // TODO: Define constructors and getters for sf::Vectors of all types

        vec2 operator+(const vec2 &other) const { return {x + other.x, y + other.y}; }
        vec2 operator-(const vec2 &other) const { return {x - other.x, y - other.y}; }
        vec2 operator*(const vec2 &other) const { return {x * other.x, y * other.y}; }
        vec2 operator/(const vec2 &other) const { return {x / other.x, y / other.y}; }

        vec2 operator+(double other) const { return {x + other, y + other}; }
        vec2 operator-(double other) const { return {x - other, y - other}; }
        vec2 operator*(double other) const { return {x * other, y * other}; }
        vec2 operator/(double other) const { return {x / other, y / other}; }
        vec2 operator-() const { return {-x, -y}; }

        vec2& operator+=(const vec2 &other) { x += other.x; y += other.y; return *this; }
        vec2& operator-=(const vec2 &other) { x -= other.x; y -= other.y; return *this; }
        vec2& operator*=(const vec2 &other) { x *= other.x; y *= other.y; return *this; }
        vec2& operator/=(const vec2 &other) { x /= other.x; y /= other.y; return *this; }

        vec2& operator+=(double other) { x += other; y += other; return *this; }
        vec2& operator-=(double other) { x -= other; y -= other; return *this; }
        vec2& operator*=(double other) { x *= other; y *= other; return *this; }
        vec2& operator/=(double other) { x /= other; y /= other; return *this; }

        bool operator==(const vec2 &other) const { return x == other.x && y == other.y; }
        bool operator!=(const vec2 &other) const { return x != other.x || y != other.y; }

        double length() const { return std::sqrt(x * x + y * y); }
        double lengthSquared() const { return x * x + y * y; }          // Enough for comparisons, faster than length()

        vec2 normalized(bool returnZeroInsteadOfThrow = false) const {
            if (!returnZeroInsteadOfThrow && length() == 0) {
                throw std::runtime_error("Cannot normalize a b::vec2 with length 0");
            }
            return length() == 0 ? vec2(0, 0) : *this / length();
        }
        vec2& normalize(bool returnZeroInsteadOfThrow = false) {
            return *this = normalized(returnZeroInsteadOfThrow);
        }

        // TODO: Maybe clone SFML in terms of defining angle functions to get the angle to X or angle to another vector?

        double dot(const vec2 &other) const { return x * other.x + y * other.y; }
        double cross(const vec2 &other) const { return x * other.y - y * other.x; }

        vec2 lerp(const vec2 &other, double t) const { return *this + (other - *this) * t; }

        vec2 projected(const vec2 &other) const { return other * (dot(other) / other.lengthSquared()); }
        vec2 reflected(const vec2 &normal) const { return *this - normal * (2.0 * dot(normal)); }

        vec2 rotated(double angle) const {
            return {x * std::cos(angle) - y * std::sin(angle), x * std::sin(angle) + y * std::cos(angle)};
        }

        vec2& rotate(double angle) { return *this = rotated(angle); }

        vec2 perpendicular() const { return {-y, x}; }
        vec2& makePerpendicular() { return *this = perpendicular(); }

#ifdef B_VEC_INTEGRATE_SFML
        operator sf::Vector2f() const { return {static_cast<float>(x), static_cast<float>(y)}; }
        operator sf::Vector2i() const { return {static_cast<int>(x), static_cast<int>(y)}; }
        operator sf::Vector2u() const { return {static_cast<unsigned int>(x), static_cast<unsigned int>(y)}; }
#endif

#ifdef B_VEC_INTEGRATE_GLM
        operator glm::vec2() const { return {static_cast<float>(x), static_cast<float>(y)}; }
        operator glm::ivec2() const { return {static_cast<int>(x), static_cast<int>(y)}; }
        operator glm::uvec2() const { return {static_cast<unsigned int>(x), static_cast<unsigned int>(y)}; }
#endif
    };

} // namespace b