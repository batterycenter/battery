
#include "battery/graphics/windowed_application.hpp"
#include "battery/graphics/window.hpp"
#include "battery/graphics/graphics_constants.hpp"
#include "battery/graphics/styles.hpp"
#include "battery/core/string.hpp"
#include "battery/core/constants.hpp"
#include "imgui_internal.h"

#include "battery/core/environment.hpp"
#ifdef BATTERY_ARCH_WINDOWS
#include <Windows.h>
#include <dwmapi.h>
#endif

namespace b {

    window::window(const std::u8string& title, sf::Vector2u size, uint32_t style, const sf::ContextSettings& settings) {
        this->create(sf::VideoMode(size), b::to_u32(title), style, settings);

        auto icon = b::resource::from_base64(b::constants::battery_icon_base64());
        sf::Image image;
        if (image.loadFromMemory(icon.data(), icon.size())) {
            (void)getWindow().setIcon(image);
        }

        if (!ImGui::SFML::Init(getWindow())) {
            throw std::runtime_error("Failed to initialize ImGui-SFML");
        }

        b::make_default_themes_available();
        b::load_default_fonts();
        b::load_theme("default");
    }

    void window::init(py::function python_ui_loop) {
        this->python_ui_loop = python_ui_loop;
    }

    void window::load_py_script(const b::resource& script) {
        ui_script = script;
        ui_script_loaded = false;
    }

    static void recover_imgui_font_stack() {
        ImGuiContext& g = *GImGui;
        ImGuiStackSizes* stack_sizes = &g.CurrentWindowStack.back().StackSizesOnBegin;
        while (g.FontStack.Size > stack_sizes->SizeOfFontStack) {
            ImGui::PopFont();
        }
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

        sf::Event event {};
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
        ImGui::SFML::Update(getWindow(), delta_clock.restart());

        // Load python if not loaded already
        if (!ui_script.as_string().empty() && !ui_script_loaded) {
            ui_script_loaded = true;
            error = {};
#ifndef BATTERY_PRODUCTION_MODE
            b::log::info("Loading b::window python ui script");
#endif
            try {
                b::py::exec(ui_script.as_string());
            }
            catch (const std::exception &e) {
                ImGui::ErrorCheckEndFrameRecover(nullptr);
                recover_imgui_font_stack();
                error = e.what();
                b::log::error("Unhandled exception:\n{}", e.what());
            }
        }

        // And then render
        if (error.has_value()) {
            render_error_message(error.value());    // Py init error
        }
        else {
            try {
                b::push_font("default");
                style.push();

                update();

                if (python_ui_loop) {
                    python_ui_loop();
                }

                style.pop();
                b::pop_font();
            }
            catch (const std::exception &e) {
                ImGui::ErrorCheckEndFrameRecover(nullptr);
                recover_imgui_font_stack();
                render_error_message(e.what());     // Py ui loop error
                b::log::error("Unhandled exception:\n{}", e.what());
            }
        }

        ImGui::SFML::Render(getWindow());
        getWindow().display();
    }

    void window::render_error_message(const std::string& error) {
        b::push_font("default");
        error_window.position = { 0, 0 };
        error_window.size = { (float)getSize().x, (float)getSize().y };
        error_window.flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
        error_window.border_width = 0.f;
        error_window.style["window-background-color"] = "#333333"_u;
        error_text.label = fmt::format("Unhandled exception:\n{}", error);
        error_text.style["text-color"] = "#D34040"_u;

        error_window([this]() {
            error_text();
        });
        b::pop_font();
    }

}