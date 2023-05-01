
#include "App.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

const int windowWidth = 800;
const int windowHeight = 600;
sf::Font font;

void App::setup() {

    window.create(sf::VideoMode({ windowWidth, windowHeight }), "Hello, world!");

    if (!font.loadFromFile("C:\\Users\\zachs\\Downloads\\fontawesome-free-6.4.0-web\\fontawesome-free-6.4.0-web\\webfonts\\fa-solid-900.ttf")) {
        return;
    }

}

void App::update() {

    const float angleIncrement = 0.0003f;
    const float radius = 0.0f;
    const float centerPosX = windowWidth / 2;
    const float centerPosY = windowHeight / 2;

    if (!window.isOpen()) {
        this->stop_application();
    }

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    angle += angleIncrement;

    float x = std::cos(angle) * radius;
    float y = std::sin(angle) * radius;

    sf::Text text(font, b::to_osstring("Hello \uf004 world!").c_str(), 36);
    text.setFillColor(sf::Color::Red);

    // Create a text object
    sf::Text text1(font, "\uf007", 36);
    text1.setFillColor(sf::Color::White);
    text1.setStyle(sf::Text::Bold);

    // Center the text in the window
    sf::FloatRect textRect = text1.getLocalBounds();
    text1.setOrigin({ textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f });
    text1.setPosition({ window.getSize().x / 2.0f, window.getSize().y / 2.0f });


    text.setPosition({ centerPosX + x, centerPosY + y });
    text.setRotation(sf::radians(angle));

    window.clear(sf::Color::Black);
    window.draw(text);
    window.draw(text1);
    window.display();
}

void App::cleanup() {

}
