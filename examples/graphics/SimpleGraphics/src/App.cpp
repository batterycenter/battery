
#include "App.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

// TODO: Resizing the window destroys everything

void MainWindow::onAttach() {

    this->create({ 800, 600 }, "Battery SimpleGraphics Example");
    if (!font.loadFromFile("C:\\Users\\zachs\\Downloads\\fontawesome-free-6.4.0-web\\fontawesome-free-6.4.0-web\\webfonts\\fa-solid-900.ttf")) {
        return;
    }

    auto img = b::Resource::FromBase64(b::Constants::BatteryIconBase64(), "png");
    if (!battery.loadFromMemory(img.data(), img.size())) {
        return;
    }

}

template<typename T>
void drawBounds(sf::RenderWindow& window, T obj) {
    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(1.0f);
    rect.setOrigin(obj.getOrigin());
    rect.setSize({ 50, 50 });
    rect.setPosition(obj.getPosition());
    window.draw(rect);
}

void MainWindow::onUpdate() {

    float speed = 150.0f;
    uint32_t fontSize = 36;
    float padding = 10.0f;
    b::vec2 batteryOffset = { 15.0f, 10.0f };

    sf::Text t1(font, "I", fontSize);
    t1.setFillColor(sf::Color::Green);

    sf::Text t2(font, U"\uf004", fontSize);
    t2.setFillColor(sf::Color::Red);

    sf::Sprite t3(battery);
    t3.setScale({ 0.5f, 0.5f });

    t2.setOrigin({ t2.getLocalBounds().width / 2.0f, t2.getLocalBounds().height / 2.0f });
    t1.setOrigin({ t1.getLocalBounds().width + t2.getLocalBounds().width / 2.0f + padding, t1.getLocalBounds().height / 2.0f });
    t3.setOrigin({ -t2.getLocalBounds().width / 2.0f - padding - batteryOffset.x, t3.getLocalBounds().height / 2.0f - batteryOffset.y });

    b::vec2 totalBounds = {
            t1.getLocalBounds().width + t2.getLocalBounds().width + t3.getLocalBounds().width - padding,
            std::max(t1.getLocalBounds().height, std::max(t2.getLocalBounds().height, t3.getLocalBounds().height) - padding)
    };

    position += velocity * speed * (float)m_frametime;

    b::vec2 minimum = { position.x - totalBounds.x / 2.0f, position.y - totalBounds.y / 2.0f };
    b::vec2 maximum = { position.x + totalBounds.x / 2.0f, position.y + totalBounds.y / 2.0f };

    if (minimum.x < 0 || maximum.x > this->getSize().x) {
        position -= velocity * speed * (float)m_frametime;
        velocity.x *= -1;
    }
    if (minimum.y < 0 || maximum.y > this->getSize().y) {
        position -= velocity * speed * (float)m_frametime;
        velocity.y *= -1;
    }

    t1.setPosition(position);
    t2.setPosition(position);
    t3.setPosition(position);

    this->clear(sf::Color::Black);
    this->draw(t1);
    this->draw(t2);
    this->draw(t3);
}

void MainWindow::onDetach() {

}
