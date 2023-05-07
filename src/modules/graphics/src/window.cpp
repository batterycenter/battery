
#include "battery/graphics/windowed_application.hpp"
#include "battery/graphics/window.hpp"
#include "battery/graphics/constants.hpp"
#include "battery/graphics/styles.hpp"
#include "battery/core/string.hpp"

namespace b {

    window::window(const std::u8string& title, sf::Vector2u size) {
        this->create(sf::VideoMode(size), b::to_u32(title));

        if (!ImGui::SFML::Init(getWindow())) {
            throw std::runtime_error("Failed to initialize ImGui-SFML");
        }

        b::push_default_style(style);
    }

    window::~window() {
        ImGui::SFML::Shutdown();
    }

    void window::_update() {
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

        ImGui::SFML::Update(getWindow(), deltaClock.restart());

        getWindow().clear(b::graphics_constants::battery_default_background_color());

        style.push();
        update();
        style.pop();

        //python();

        ImGui::SFML::Render(getWindow());
        getWindow().display();
    }

}