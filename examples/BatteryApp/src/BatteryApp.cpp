
#include "pch.h"
#include "BatteryApp.h"
#include "Battery/Extern/magic_enum.hpp"

#include "windows.h"
#include "shellapi.h"

std::vector<sf::Image> sprites_idle;
std::vector<sf::Image> sprites_attack;
std::vector<sf::Image> sprites_death;

std::vector<sf::Image>* currentAnimation = &sprites_idle;
int sprite = 0;

void loadAnimation(const sf::Image& image, std::vector<sf::Image>& target, int sprites, int leftOffset, int topOffset, int offset) {

    for (int i = 0; i < sprites; i++) {
        sf::Image im;
        im.create(24, 30);
        im.copy(image, 0, 0, {leftOffset + i * offset, topOffset, 24, 30});

        // Remove background
        for (int y = 0; y < im.getSize().y; y++) {
            for (int x = 0; x < im.getSize().x; x++) {
                if (im.getPixel(x, y) == image.getPixel(0, 0)) {
                    im.setPixel(x, y, sf::Color(0, 0, 0, 0));
                }
            }
        }

        target.emplace_back(im);
    }
}

void loadSprites() {
    sf::Image sprite_raw;

    if (!sprite_raw.loadFromFile("resources/KnightIdle_strip.png"))
        throw UI_EXCEPTION("Oh no");
    loadAnimation(sprite_raw, sprites_idle, 15, 39, 14, 96);

    if (!sprite_raw.loadFromFile("resources/KnightAttack_strip.png"))
        throw UI_EXCEPTION("Oh no");
    loadAnimation(sprite_raw, sprites_attack, 22, 55, 14, 144);

    if (!sprite_raw.loadFromFile("resources/KnightDeath_strip.png"))
        throw UI_EXCEPTION("Oh no");
    loadAnimation(sprite_raw, sprites_death, 15, 39, 14, 96);
}

void BatteryApp::OnStartup() {
    using namespace Battery;

    loadSprites();

    auto im = Battery::DecodeBase64(BATTERY_DEFAULT_WINDOW_ICON_BASE64);
    sf::Image image;
    image.loadFromMemory(&im[0], im.size());

    tray = std::make_unique<Battery::TrayIcon>(image, "sebi is dick");

    tray->attachLeftClickCallback([&] { sprite = 0; currentAnimation = &sprites_attack; });
    tray->attachRightClickCallback([&] { sprite = 0; currentAnimation = &sprites_death; });
}

void BatteryApp::OnUpdate() {

    static int cnt = 0;
    cnt++;

    tray->update();

    if (cnt % 5 == 0) {
        sprite++;
        if (sprite >= currentAnimation->size()) {
            sprite = 0;
            currentAnimation = &sprites_idle;
        }
        tray->setIcon((*currentAnimation)[sprite]);
    }

}

void BatteryApp::OnRender() {

    ImGui::SetNextWindowPos({ cos(Battery::GetRuntime().asSeconds() * 5) * 100 + 100, sin(Battery::GetRuntime().asSeconds() * 5) * 100 + 100 });
    ImGui::Begin("test");
    ImGui::End();

    ImGui::ShowDemoWindow();
    
}
