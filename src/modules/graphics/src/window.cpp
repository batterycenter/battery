
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

    void window::create(const sf::Vector2u& mode, const b::string& title, std::uint32_t style, const sf::ContextSettings& settings) {
        create(sf::VideoMode(mode), title, style, settings);
    }

    void window::create(sf::VideoMode mode, const b::string& title, std::uint32_t style, const sf::ContextSettings& settings) {
        m_sfmlWindow.create(mode, title, style, settings);

        if (m_firstWindowCreation) {
            auto icon = b::resource::from_base64(b::constants::battery_icon_base64());
            sf::Image image;
            if (image.loadFromMemory(icon.data(), icon.size())) {
                (void)m_sfmlWindow.setIcon(image);
            }

            if (!ImGui::SFML::Init(m_sfmlWindow)) {
                throw std::runtime_error("Failed to initialize ImGui-SFML");
            }

            b::make_default_themes_available();     // TODO: Move these to a better place
            b::load_default_fonts();

            m_firstWindowCreation = false;
        }
    }

    void window::init(py::function callback) {
        this->m_pythonUiLoopCallback = callback;
    }

    void window::load_py_script(const b::resource& script) {
        m_uiScriptResource = script;
        m_uiScriptLoaded = false;
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

        if (!m_sfmlWindow.isOpen()) {
            return;
        }

#ifdef BATTERY_ARCH_WINDOWS
        if (m_useWin32ImmersiveDarkMode != m_win32IDMActive) {
            BOOL USE_DARK_MODE = m_useWin32ImmersiveDarkMode;
            BOOL SET_IMMERSIVE_DARK_MODE_SUCCESS = SUCCEEDED(DwmSetWindowAttribute(
                    m_sfmlWindow.getSystemHandle(), DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
                    &USE_DARK_MODE, sizeof(USE_DARK_MODE)));
            m_win32IDMActive = m_useWin32ImmersiveDarkMode;
            if (!SET_IMMERSIVE_DARK_MODE_SUCCESS) {
                throw std::runtime_error("Failed to set immersive dark mode on b::window");
            }
        }
#endif

        sf::Event event {};
        while (m_sfmlWindow.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(m_sfmlWindow, event);

            if (event.type == sf::Event::Closed) {
                m_sfmlWindow.close();
            }
            else if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea({ 0, 0 }, { (float)event.size.width, (float)event.size.height });
                m_sfmlWindow.setView(sf::View(visibleArea));
            }
        }

        b::update_themes();

        // Load python if not loaded already
        if (!m_uiScriptResource.string().empty() && !m_uiScriptLoaded) {
            m_uiScriptLoaded = true;
            m_errorMessage = {};
#ifndef BATTERY_PRODUCTION_MODE
            b::log::info("Loading b::window python ui script");
#endif
            try {
                b::py::exec(m_uiScriptResource.string());
            }
            catch (const std::exception &e) {
                ImGui::ErrorCheckEndFrameRecover(nullptr);
                recover_imgui_font_stack();
                m_errorMessage = e.what();
                b::log::error("Unhandled exception:\n{}", e.what());
            }
        }

        m_sfmlWindow.clear(b::graphics_constants::battery_default_background_color());
        ImGui::SFML::Update(m_sfmlWindow, m_deltaClock.restart());
        b::lock_font_stack();

        // And then render
        if (m_errorMessage.has_value()) {
            render_error_message(m_errorMessage.value());    // Py init error
        }
        else {
            try {
                b::push_font("default");
                style.push();

                update();

                if (m_pythonUiLoopCallback) {
                    m_pythonUiLoopCallback();
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
        ImGui::SFML::Render(m_sfmlWindow);
        m_sfmlWindow.display();
    }

    void window::render_error_message(const b::string& error) {
        m_errorWindowWidget.children_style.font = "default";  // Ask it to explicitly push the default font, in case it wasn't cleaned up properly
        m_errorWindowWidget.left = 0;
        m_errorWindowWidget.top = 0;
        m_errorWindowWidget.width = static_cast<float>(getSize().x);
        m_errorWindowWidget.height = static_cast<float>(getSize().y);
        m_errorWindowWidget.flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
        m_errorWindowWidget.style["ImGuiCol_WindowBg"] = "#333333"_u;
        m_errorWindowWidget.style["ImGuiStyleVar_WindowRounding"] = 0;
        m_errorTextWidget.label = b::format("Unhandled exception:\n{}", error);
        m_errorTextWidget.style["ImGuiCol_Text"] = "#D34040"_u;

        m_errorWindowWidget([this]() {
            m_errorTextWidget();
        });
    }

}