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

    sf::RenderWindow window;
    float angle = 0.0f;

    void setup() override;
    void update() override;
    void cleanup() override;
};
