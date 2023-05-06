
#include "battery/core/string.hpp"
#include "battery/graphics/graphics_application.hpp"

namespace b {

    graphics_application::graphics_application(const std::u8string& title, sf::Vector2u size)
        : title(title), size(size) {
        set_requested_framerate(60.0);
    }


    void graphics_application::app_setup() {

        window.create(sf::VideoMode(size), b::to_u32(title));

        if (!ImGui::SFML::Init(window)) {
            throw std::runtime_error("Failed to initialize ImGui-SFML");
        }
        setup();
    }

    void graphics_application::app_update() {

        if (!window.isOpen()) {
            this->stop_application();
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea({ 0, 0 }, { (float)event.size.width, (float)event.size.height });
                window.setView(sf::View(visibleArea));
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear(sf::Color::Black);
        update();

        //python();

        ImGui::SFML::Render(window);
        window.display();
    }

    void graphics_application::app_cleanup() {
        cleanup();
        ImGui::SFML::Shutdown();
    }

}
