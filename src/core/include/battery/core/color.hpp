#pragma once

#ifdef B_COLOR_INTEGRATE_SFML
#include "SFML/Graphics/Color.hpp"
#endif

#ifdef B_COLOR_INTEGRATE_IMGUI
#include "imgui.h"
#endif

#ifdef B_COLOR_INTEGRATE_GLM
#include "glm/vec4.hpp"
#endif

#ifdef B_COLOR_INTEGRATE_NLOHMANN_JSON
#include "battery/core/extern/json.hpp"
#endif

// You can set B_COLOR_INTEGRATE_CUSTOM
// to your custom vector type conversion functions

#include "battery/core/vec.hpp"
#include "battery/core/log.hpp"

namespace b {

    // sf::Color is range 0-255, while ImColor/ImVec4 and b::Color are range 0.0 - 1.0
    struct Color {
        double r = 0.0;
        double g = 0.0;
        double b = 0.0;
        double a = 1.0;

        Color() = default;
        Color(double r, double g, double b, double a = 1.0) : r(r), g(g), b(b), a(a) {}

        Color operator+(const Color &other) const { return { r + other.r, g + other.g, b + other.b, a + other.a }; }
        Color operator-(const Color &other) const { return { r - other.r, g - other.g, b - other.b, a - other.a }; }
        Color operator*(const Color &other) const { return { r * other.r, g * other.g, b * other.b, a * other.a }; }
        Color operator/(const Color &other) const { return { r / other.r, g / other.g, b / other.b, a / other.a }; }

        Color operator+(double other) const { return { r + other, g + other, b + other, a + other }; }
        Color operator-(double other) const { return { r - other, g - other, b - other, a - other }; }
        Color operator*(double other) const { return { r * other, g * other, b * other, a * other }; }
        Color operator/(double other) const { return { r / other, g / other, b / other, a / other }; }

        Color& operator+=(const Color &other) { r += other.r; g += other.g; b += other.b; a += other.a; return *this; }
        Color& operator-=(const Color &other) { r -= other.r; g -= other.g; b -= other.b; a -= other.a; return *this; }
        Color& operator*=(const Color &other) { r *= other.r; g *= other.g; b *= other.b; a *= other.a; return *this; }
        Color& operator/=(const Color &other) { r /= other.r; g /= other.g; b /= other.b; a /= other.a; return *this; }

        Color& operator+=(double other) { r += other; g += other; b += other; a += other; return *this; }
        Color& operator-=(double other) { r -= other; g -= other; b -= other; a -= other; return *this; }
        Color& operator*=(double other) { r *= other; g *= other; b *= other; a *= other; return *this; }
        Color& operator/=(double other) { r /= other; g /= other; b /= other; a /= other; return *this; }

        bool operator==(const Color &other) const { return r == other.r && g == other.g && b == other.b && a == other.a; }
        bool operator!=(const Color &other) const { return r != other.r || g != other.g || b != other.b || a != other.a; }

        [[nodiscard]] Color lerp(const Color& other, double t) const { return *this + (other - *this) * t; }

#ifdef B_COLOR_INTEGRATE_SFML
        Color(const sf::Color& other) : r(other.r / 255.0), g(other.g / 255.0), b(other.b / 255.0), a(other.a / 255.0) {}

        Color& operator=(const sf::Color& other) {
            r = other.r / 255.0;
            g = other.g / 255.0;
            b = other.b / 255.0;
            a = other.a / 255.0;
            return *this;
        }

        operator sf::Color() const {
            return {
                static_cast<uint8_t>(r * 255),
                static_cast<uint8_t>(g * 255),
                static_cast<uint8_t>(b * 255),
                static_cast<uint8_t>(a * 255)
            };
        }
#endif

#ifdef B_COLOR_INTEGRATE_IMGUI
        Color(const ImVec4& other) : r(other.x), g(other.y), b(other.z), a(other.w) {}

        Color& operator=(const ImVec4& other) {
            r = other.x;
            g = other.y;
            b = other.z;
            a = other.w;
            return *this;
        }

        operator ImVec4() const {
            return ImVec4(r, g, b, a);
        }
#endif

#ifdef B_COLOR_INTEGRATE_GLM
        Color(const glm::vec4& other) : r(other.x), g(other.y), b(other.z), a(other.w) {}

        Color& operator=(const glm::vec4& other) {
            r = other.x;
            g = other.y;
            b = other.z;
            a = other.w;
            return *this;
        }

        operator glm::vec4() const {
            return glm::vec4(r, g, b, a);
        }
#endif

#ifdef B_COLOR_INTEGRATE_NLOHMANN_JSON
        Color(const nlohmann::json& other) : r(other[0]), g(other[1]), b(other[2]), a(other[3]) {}

        Color& operator=(const nlohmann::json& other) {
            r = other[0];
            g = other[1];
            b = other[2];
            a = other[3];
            return *this;
        }

        operator nlohmann::json() const {
            return { r, g, b, a };
        }

        friend void to_json(nlohmann::json& j, const Color& c) {
            j = nlohmann::json{ c.r, c.g, c.b, c.a };
        }

        friend void from_json(const nlohmann::json& j, Color& c) {
            c.r = j[0];
            c.g = j[1];
            c.b = j[2];
            c.a = j[3];
        }
#endif

#ifdef B_COLOR_INTEGRATE_CUSTOM
        B_COLOR_INTEGRATE_CUSTOM  // This is a macro that can be defined by the user to integrate with their own color types
#endif
    };

} // namespace b

template <> struct fmt::formatter<b::Color> {
    constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator {
        return ctx.begin();
    }
    auto format(const b::Color& color, format_context& ctx) const -> format_context::iterator {
        return fmt::format_to(ctx.out(), "[{:.3f}, {:.3f}, {:.3f}, {:.3f}]", color.r, color.g, color.b, color.a);
    }
};