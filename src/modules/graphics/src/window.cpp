
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

    void basic_window::create(const sf::Vector2u& mode, const b::string& title, std::uint32_t style, const sf::ContextSettings& settings) {
        create(sf::VideoMode(mode), title, style, settings);
    }

    void basic_window::create(sf::VideoMode mode, const b::string& title, std::uint32_t style, const sf::ContextSettings& settings) {
        m_sfmlWindow.create(mode, title, style, settings);

        if (m_firstWindowCreation) {
            if (!ImGui::SFML::Init(m_sfmlWindow)) {
                throw std::runtime_error("Failed to initialize ImGui-SFML");
            }

            b::make_default_themes_available();     // TODO: Move these to a better place
            b::load_default_fonts();

            m_firstWindowCreation = false;
        }

        auto icon = b::resource::from_base64(b::constants::battery_icon_base64());
        sf::Image image;
        if (image.loadFromMemory(icon.data(), icon.size())) {
            (void)m_sfmlWindow.setIcon(image);
        }
    }

    void basic_window::pyInit(py::function callback) {
        this->m_pythonUiLoopCallback = callback;
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

    void basic_window::invoke_update() {

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

            switch (event.type) {
                case sf::Event::Closed:
                    if (!dispatchEvent<b::events::WindowCloseEvent>()) {
                        m_sfmlWindow.close();
                    }
                    break;

                case sf::Event::Resized:
                    m_sfmlWindow.setView(sf::View(sf::FloatRect({ 0, 0 }, { static_cast<float>(event.size.width), static_cast<float>(event.size.height) })));
                    dispatchEvent<b::events::WindowResizeEvent>(event.size.width, event.size.height);
                    break;

                case sf::Event::LostFocus:
                    dispatchEvent<b::events::WindowLostFocusEvent>();
                    break;

                case sf::Event::GainedFocus:
                    dispatchEvent<b::events::WindowGainedFocusEvent>();
                    break;

                case sf::Event::KeyPressed:
                    dispatchEvent<b::events::KeyPressEvent>(event.key);
                    break;

                case sf::Event::KeyReleased:
                    dispatchEvent<b::events::KeyReleaseEvent>(event.key);
                    break;

                case sf::Event::TextEntered:
                    dispatchEvent<b::events::TextEnteredEvent>(event.text);
                    break;

                case sf::Event::MouseWheelScrolled:
                    dispatchEvent<b::events::MouseWheelScrollEvent>(event.mouseWheelScroll);
                    break;

                case sf::Event::MouseButtonPressed:
                    dispatchEvent<b::events::MouseButtonPressEvent>(event.mouseButton);
                    break;

                case sf::Event::MouseButtonReleased:
                    dispatchEvent<b::events::MouseButtonReleaseEvent>(event.mouseButton);
                    break;

                case sf::Event::MouseMoved:
                    dispatchEvent<b::events::MouseMoveEvent>(event.mouseMove);
                    break;

                case sf::Event::MouseEntered:
                    dispatchEvent<b::events::MouseEnteredWindowEvent>();
                    break;

                case sf::Event::MouseLeft:
                    dispatchEvent<b::events::MouseLeftWindowEvent>();
                    break;

                case sf::Event::JoystickButtonPressed:
                    dispatchEvent<b::events::JoystickButtonPressEvent>(event.joystickButton);
                    break;

                case sf::Event::JoystickButtonReleased:
                    dispatchEvent<b::events::JoystickButtonReleaseEvent>(event.joystickButton);
                    break;

                case sf::Event::JoystickMoved:
                    dispatchEvent<b::events::JoystickMoveEvent>(event.joystickMove);
                    break;

                case sf::Event::JoystickConnected:
                    dispatchEvent<b::events::JoystickConnectEvent>(event.joystickConnect);
                    break;

                case sf::Event::JoystickDisconnected:
                    dispatchEvent<b::events::JoystickDisconnectEvent>(event.joystickConnect);
                    break;

                case sf::Event::TouchBegan:
                    dispatchEvent<b::events::TouchBeginEvent>(event.touch);
                    break;

                case sf::Event::TouchMoved:
                    dispatchEvent<b::events::TouchMoveEvent>(event.touch);
                    break;

                case sf::Event::TouchEnded:
                    dispatchEvent<b::events::TouchEndEvent>(event.touch);
                    break;

                case sf::Event::SensorChanged:
                    dispatchEvent<b::events::SensorChangeEvent>(event.sensor);
                    break;

                default:
                    break;
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

            b::log::info("Loaded successfully");
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

    void basic_window::render_error_message(const b::string& error) {
        m_errorPanelWidget.children_style.font = "default";  // Ask it to explicitly push the default font, in case it wasn't cleaned up properly
        m_errorPanelWidget.left = 0;
        m_errorPanelWidget.top = 0;
        m_errorPanelWidget.width = static_cast<float>(getSize().x);
        m_errorPanelWidget.height = static_cast<float>(getSize().y);
        m_errorPanelWidget.flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
        m_errorPanelWidget.style["ImGuiCol_WindowBg"] = "#333333"_u;
        m_errorPanelWidget.style["ImGuiStyleVar_WindowRounding"] = 0;
        m_errorTextWidget.label = b::format("Unhandled exception:\n{}", error);
        m_errorTextWidget.left = "5px"_u;
        m_errorTextWidget.top = "5px"_u;
        m_errorTextWidget.right = "5px"_u;
        m_errorTextWidget.bottom = "5px"_u;
        m_errorTextWidget.alignv = b::alignv::TOP;
        m_errorTextWidget.style["ImGuiCol_Text"] = "#D34040"_u;

        m_errorPanelWidget([this]() {
            m_errorTextWidget();
        });
    }

}