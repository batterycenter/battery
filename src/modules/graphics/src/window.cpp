
#include "battery/graphics/windowed_application.hpp"
#include "battery/graphics/window.hpp"
#include "battery/graphics/graphics_constants.hpp"
#include "battery/graphics/styles.hpp"
#include "battery/core/string.hpp"

#include "battery/core/environment.hpp"
#ifdef BATTERY_ARCH_WINDOWS
#include <Windows.h>
#include <dwmapi.h>
#endif

namespace b {

    window::window(const std::u8string& title, sf::Vector2u size, uint32_t style, const sf::ContextSettings& settings) {
        this->create(sf::VideoMode(size), b::to_u32(title), style, settings);

        if (!ImGui::SFML::Init(getWindow())) {
            throw std::runtime_error("Failed to initialize ImGui-SFML");
        }

        b::make_default_themes_available();
        b::load_default_fonts();
        b::load_theme("default");
    }

    void window::_update() {

#ifdef BATTERY_ARCH_WINDOWS
        if (win32_use_immersive_dark_mode != win32_idm_used) {
            BOOL USE_DARK_MODE = win32_use_immersive_dark_mode;
            BOOL SET_IMMERSIVE_DARK_MODE_SUCCESS = SUCCEEDED(DwmSetWindowAttribute(
                    getWindow().getSystemHandle(), DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
                    &USE_DARK_MODE, sizeof(USE_DARK_MODE)));
            win32_idm_used = win32_use_immersive_dark_mode;
            if (!SET_IMMERSIVE_DARK_MODE_SUCCESS) {
                throw std::runtime_error("Failed to set immersive dark mode on b::window");
            }
        }
#endif

        sf::Event event;
        while (getWindow().pollEvent(event)) {
            ImGui::SFML::ProcessEvent(getWindow(), event);

            if (event.type == sf::Event::Closed) {
                getWindow().close();
            }
            else if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea({ 0, 0 }, { (float)event.size.width, (float)event.size.height });
                getWindow().setView(sf::View(visibleArea));
            }
        }

        getWindow().clear(b::graphics_constants::battery_default_background_color());
        ImGui::SFML::Update(getWindow(), deltaClock.restart());

        b::push_font("default");
        style.push();
        update();
        style.pop();
        b::pop_font();

        //python();

        ImGui::SFML::Render(getWindow());
        getWindow().display();
    }

}