#pragma once

#ifdef B_COLOR_INTEGRATE_SFML
#include "SFML/System/Color.hpp"
#endif

#ifdef B_COLOR_INTEGRATE_IMGUI
#include "imgui.h"
#endif

#ifdef B_COLOR_INTEGRATE_GLM
#include "glm/vec4.hpp"
#endif

// You can set B_COLOR_INTEGRATE_CUSTOM
// to your custom vector type conversion functions

namespace b {

    struct Color {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float a = 1.0f;

        Color() = default;


#ifdef B_COLOR_INTEGRATE_SFML
        vec2(const sf::Vector2f &other) : x(other.x), y(other.y) {}
        vec2(const sf::Vector2i &other) : x(other.x), y(other.y) {}
        vec2(const sf::Vector2u &other) : x(other.x), y(other.y) {}
        vec2& operator=(const sf::Vector2f &other) { x = other.x; y = other.y; return *this; }
        vec2& operator=(const sf::Vector2i &other) { x = other.x; y = other.y; return *this; }
        vec2& operator=(const sf::Vector2u &other) { x = other.x; y = other.y; return *this; }
        operator sf::Vector2f() const { return {static_cast<float>(x), static_cast<float>(y)}; }
        operator sf::Vector2i() const { return {static_cast<int>(x), static_cast<int>(y)}; }
        operator sf::Vector2u() const { return {static_cast<unsigned int>(x), static_cast<unsigned int>(y)}; }
#endif

#ifdef B_COLOR_INTEGRATE_IMGUI
        vec2(const ImVec2 &other) : x(other.x), y(other.y) {}
        vec2& operator=(const ImVec2 &other) { x = other.x; y = other.y; return *this; }
        operator ImVec2() const { return {static_cast<float>(x), static_cast<float>(y)}; }
#endif

#ifdef B_COLOR_INTEGRATE_GLM
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

#ifdef B_COLOR_INTEGRATE_CUSTOM
        B_VEC_INTEGRATE_CUSTOM  // This is a macro that can be defined by the user to integrate with their own vector types
#endif
    };

} // namespace b