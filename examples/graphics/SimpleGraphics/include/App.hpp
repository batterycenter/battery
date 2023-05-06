#pragma once

#include "battery/core/application.hpp"
#include "battery/core/log.hpp"
#include "battery/core/main.hpp"
#include "battery/core/all.hpp"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"
#include "SFML/Window.hpp"

class App : public b::application {
public:
    App() = default;
    ~App() = default;

    sf::Font font;
    sf::Texture battery;
    sf::RenderWindow window;

    sf::Vector2<float> position = { 100.0f, 100.0f };
    sf::Vector2<float> velocity = { 1.0f, 1.0f };

    void setup() override;
    void update() override;
    void cleanup() override;
};
