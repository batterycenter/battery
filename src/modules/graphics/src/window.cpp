
#include "battery/graphics/application.hpp"
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

    window::window(const b::string& title, sf::Vector2u size, uint32_t style, const sf::ContextSettings& settings) {
        this->create(sf::VideoMode(size), title, style, settings);

        auto icon = b::resource::from_base64(b::constants::battery_icon_base64());
        sf::Image image;
        if (image.loadFromMemory(icon.data(), icon.size())) {
            (void)sfml_window.setIcon(image);
        }

        if (!ImGui::SFML::Init(sfml_window)) {
            throw std::runtime_error("Failed to initialize ImGui-SFML");
        }

        b::make_default_themes_available();
        b::load_default_fonts();
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

        if (g.CurrentWindowStack.empty()) {
            return;
        }

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
                    sfml_window.getSystemHandle(), DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
                    &USE_DARK_MODE, sizeof(USE_DARK_MODE)));
            win32_idm_used = win32_use_immersive_dark_mode;
            if (!SET_IMMERSIVE_DARK_MODE_SUCCESS) {
                throw std::runtime_error("Failed to set immersive dark mode on b::window");
            }
        }
#endif

        sf::Event event {};
        while (sfml_window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(sfml_window, event);

            if (event.type == sf::Event::Closed) {
                sfml_window.close();
            }
            else if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea({ 0, 0 }, { (float)event.size.width, (float)event.size.height });
                sfml_window.setView(sf::View(visibleArea));
            }
        }

        b::update_themes();

        // Load python if not loaded already
        if (!ui_script.string().empty() && !ui_script_loaded) {
            ui_script_loaded = true;
            error = {};
#ifndef BATTERY_PRODUCTION_MODE
            b::log::info("Loading b::window python ui script");
#endif
            try {
                b::py::exec(ui_script.string());
            }
            catch (const std::exception &e) {
                ImGui::ErrorCheckEndFrameRecover(nullptr);
                recover_imgui_font_stack();
                error = e.what();
                b::log::error("Unhandled exception:\n{}", e.what());
            }
        }

        sfml_window.clear(b::graphics_constants::battery_default_background_color());
        ImGui::SFML::Update(sfml_window, delta_clock.restart());
        b::lock_font_stack();

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

        b::unlock_font_stack();
        ImGui::SFML::Render(sfml_window);
        sfml_window.display();
    }

    void window::render_error_message(const b::string& error) {
        error_window.children_style.font = "default";  // Ask it to explicitly push the default font, in case it wasn't cleaned up properly
        error_window.left = 0;
        error_window.top = 0;
        error_window.width = static_cast<float>(getSize().x);
        error_window.height = static_cast<float>(getSize().y);
        error_window.flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
        error_window.style["ImGuiCol_WindowBg"] = "#333333"_u;
        error_window.style["ImGuiStyleVar_WindowRounding"] = 0;
        error_text.label = b::format("Unhandled exception:\n{}", error);
        error_text.style["ImGuiCol_Text"] = "#D34040"_u;

        error_window([this]() {
            error_text();
        });
    }

}