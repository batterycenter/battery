
#include "App.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

// TODO: Resizing the window destroys everything

void App::setup() {

    window.create(sf::VideoMode({ 800, 600 }), "Hello, world!");
    if (!font.loadFromFile("C:\\Users\\zachs\\Downloads\\fontawesome-free-6.4.0-web\\fontawesome-free-6.4.0-web\\webfonts\\fa-solid-900.ttf")) {
        return;
    }

    auto img = b::resource::from_base64(b::constants::battery_icon_base64(), "png");
    if (!battery.loadFromMemory(img.data(), img.size())) {
        return;
    }

}

void App::update() {

    float speed = 50.0f;
    uint32_t fontSize = 36;
    float padding = 10.0f;
    sf::Vector2 batteryOffset = { 15.0f, 10.0f };

    if (!window.isOpen()) {
        this->stop_application();
    }

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    sf::Text t1(font, "I", fontSize);
    t1.setFillColor(sf::Color::Green);

    sf::Text t2(font, U"\uf004", fontSize);
    t2.setFillColor(sf::Color::Red);

    sf::Sprite t3(battery);
    t3.setScale({ 0.5f, 0.5f });

    t2.setOrigin({ t2.getLocalBounds().width / 2.0f, t2.getLocalBounds().height / 2.0f });
    t1.setOrigin({ t1.getLocalBounds().width + t2.getLocalBounds().width / 2.0f + padding, t1.getLocalBounds().height / 2.0f });
    t3.setOrigin({ -t2.getLocalBounds().width / 2.0f - padding - batteryOffset.x, t3.getLocalBounds().height / 2.0f - batteryOffset.y });

    sf::Vector2 totalBounds = {
            t1.getLocalBounds().width + t2.getLocalBounds().width + t3.getLocalBounds().width + padding + batteryOffset.x,
            std::max(t1.getLocalBounds().height, std::max(t2.getLocalBounds().height, t3.getLocalBounds().height))
    };

    position += velocity * speed * (float)frametime;

    sf::Vector2 minimum = { position.x - totalBounds.x / 2.0f, position.y - totalBounds.y / 2.0f };
    sf::Vector2 maximum = { position.x + totalBounds.x / 2.0f, position.y + totalBounds.y / 2.0f };

    if (minimum.x < 0 || maximum.x > window.getSize().x) {
        position -= velocity * speed * (float)frametime;
        velocity.x *= -1;
    }
    if (minimum.y < 0 || maximum.y > window.getSize().y) {
        position -= velocity * speed * (float)frametime;
        velocity.y *= -1;
    }

    t1.setPosition(position);
    t2.setPosition(position);
    t3.setPosition(position);

    window.clear(sf::Color::Black);
    window.draw(t1);
    window.draw(t2);
    window.draw(t3);

    window.display();
}

void App::cleanup() {

}
