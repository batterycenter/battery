
#include "battery/graphics/application.hpp"
#include "battery/graphics/window.hpp"
#include "battery/graphics/constants.hpp"
#include "battery/graphics/styles.hpp"
#include "battery/core/string.hpp"
#include "battery/core/constants.hpp"
#include "imgui_internal.h"

#ifdef B_OS_WINDOWS
#include <Windows.h>
#include <dwmapi.h>
#endif

namespace b {

    BaseWindow::~BaseWindow() noexcept {

        if (!m_windowPositionJsonFile.empty()) {                // TODO: Add a check that the window is not created off-monitor. This could make the window unusable.
            try {
                nlohmann::json json;       // If the file does not exist yet, just create it
                try {
                    json = nlohmann::json::parse(b::fs::read_text_file(m_windowPositionJsonFile));
                }
                catch (...) {}

                json["position"] = { m_lastWindowState.position.x, m_lastWindowState.position.y };
                json["size"] = { m_lastWindowState.size.x, m_lastWindowState.size.y };
                json["maximized"] = m_lastWindowState.maximized;
                b::fs::write_text_file(m_windowPositionJsonFile, json.dump(4));
            }
            catch (const std::exception& e) {
                b::log::core::warn("Failed to save window position to '{}': {}", m_windowPositionJsonFile.string(), e.what());
            }
        }

        ImGui::SFML::Shutdown(m_sfmlWindow);
    }

    void BaseWindow::create(const sf::Vector2u& mode, const b::string& title, std::uint32_t style, const sf::ContextSettings& settings) {
        create(sf::VideoMode(mode), title, style, settings);
    }

    void BaseWindow::create(sf::VideoMode mode, const b::string& title, std::uint32_t style, const sf::ContextSettings& settings, bool silenceJsonWarning) {
        m_sfmlWindow.create(mode, title, style, settings);

        if (!m_windowPositionJsonFile.empty() && !silenceJsonWarning) {
            b::log::core::warn("rememberWindowPositionJsonFile() was called on this window, "
                               "but create() was called with an explicit window size. "
                               "The last window position will not have any effect.");
        }

        if (m_firstWindowCreation) {
            if (!ImGui::SFML::Init(m_sfmlWindow)) {
                throw std::runtime_error("Failed to initialize ImGui-SFML");
            }

            b::make_default_themes_available();     // TODO: Move these to a better place
            b::LoadDefaultFonts();

            m_firstWindowCreation = false;
        }

        auto icon = b::Resource::FromBase64(b::Constants::BatteryIconBase64());
        sf::Image image;
        if (image.loadFromMemory(icon.data(), icon.size())) {
            (void)m_sfmlWindow.setIcon(image);
        }
    }

    void BaseWindow::create(const b::string& title, std::uint32_t style, const sf::ContextSettings& settings) {

        // Reload the window where it was last closed
        sf::Vector2u size = m_defaultWindowSize;
        sf::Vector2i position = { static_cast<int>(sf::VideoMode::getDesktopMode().size.x - size.x) / 2,
                                  static_cast<int>(sf::VideoMode::getDesktopMode().size.y - size.y) / 2 };
        bool maximized = false;

        try {
            nlohmann::json json = nlohmann::json::parse(b::fs::read_text_file(m_windowPositionJsonFile));
            size.x = json["size"][0];
            size.y = json["size"][1];
            position.x = json["position"][0];
            position.y = json["position"][1];
            maximized = json["maximized"];
        }
        catch (const std::exception& e) {
            if (b::fs::exists(m_windowPositionJsonFile)) {
                b::log::core::warn("Failed to reload window position from '{}': {}", m_windowPositionJsonFile.string(), e.what());
            }
        }

        create(sf::VideoMode(size), title, style, settings, true);
        m_sfmlWindow.setPosition(position);
        if (maximized) {
            maximize();
        }
    }

    void BaseWindow::pyInit(py::function callback) {
        this->m_pythonUiLoopCallback = callback;
    }

    void BaseWindow::rememberWindowPositionJsonFile(const b::fs::path& filename) {
        m_windowPositionJsonFile = filename;
        if (isOpen()) {
            b::log::warn("b::window::rememberWindowPositionJsonFile() called after window was already created. "
                         "This will not have any effect on the current window.");
        }
    }

    void BaseWindow::setDefaultWindowSize(const sf::Vector2u& size) {
        m_defaultWindowSize = size;
        if (isOpen()) {
            b::log::warn("b::window::rememberWindowPositionJsonFile() called after window was already created. "
                         "This will not have any effect on the current window.");
        }
    }

    void BaseWindow::showInTaskbar() {
#ifdef B_OS_WINDOWS
        long style = GetWindowLongW(getSystemHandle(), GWL_STYLE);
        style &= ~(WS_VISIBLE);
        SetWindowLongW(getSystemHandle(), GWL_STYLE, style);
        ShowWindow(getSystemHandle(), SW_SHOW);
#else
#warning "Not implemented"
#endif
    }

    void BaseWindow::hideFromTaskbar() {
#ifdef B_OS_WINDOWS
        long style = GetWindowLongW(getSystemHandle(), GWL_STYLE);
        style |= WS_VISIBLE;
        ShowWindow(getSystemHandle(), SW_HIDE);
        SetWindowLongW(getSystemHandle(), GWL_STYLE, style);
        ShowWindow(getSystemHandle(), SW_SHOW);
#else
#warning "Not implemented"
#endif
    }

    void BaseWindow::maximize() {
#ifdef B_OS_WINDOWS
        ShowWindow(getSystemHandle(), SW_MAXIMIZE);
#else
#warning "Not implemented"
#endif
    }

    void BaseWindow::minimize() {
#ifdef B_OS_WINDOWS
        ShowWindow(getSystemHandle(), SW_MINIMIZE);
#else
#warning "Not implemented"
#endif
    }

    void BaseWindow::restore() {
#ifdef B_OS_WINDOWS
        ShowWindow(getSystemHandle(), SW_RESTORE);
#else
#warning "Not implemented"
#endif
    }

    bool BaseWindow::isMaximized() {
#ifdef B_OS_WINDOWS
        return IsZoomed(getSystemHandle()) != 0;
#else
#warning "Not implemented"
#endif
    }

    bool BaseWindow::isMinimized() {
#ifdef B_OS_WINDOWS
        return IsIconic(getSystemHandle()) != 0;
#else
#warning "Not implemented"
#endif
    }

    ImVec2 BaseWindow::getMousePos() {
        return m_mousePos;
    }

    ImVec2 BaseWindow::getMousePosPrev() {
        return m_mousePosPrev;
    }

    ImVec2 BaseWindow::getMouseDelta() {
        return m_mouseDelta;
    }

    static void RecoverImguiFontStack() {
        ImGuiContext& g = *GImGui;

        if (g.CurrentWindowStack.empty()) {
            return;
        }

        ImGuiStackSizes* stackSizes = &g.CurrentWindowStack.back().StackSizesOnBegin;
        while (g.FontStack.Size > stackSizes->SizeOfFontStack) {
            ImGui::PopFont();
        }
    }

    void BaseWindow::invoke_update() {

#ifdef B_OS_WINDOWS
        if (m_useWin32ImmersiveDarkMode != m_win32IDMActive) {
            BOOL useDarkMode = static_cast<BOOL>(m_useWin32ImmersiveDarkMode);
            bool const setImmersiveDarkModeSuccess = SUCCEEDED(DwmSetWindowAttribute(
                    m_sfmlWindow.getSystemHandle(), DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
                    &useDarkMode, sizeof(useDarkMode)));
            m_win32IDMActive = m_useWin32ImmersiveDarkMode;
            if (!setImmersiveDarkModeSuccess) {
                throw std::runtime_error("Failed to set immersive dark mode on b::window");
            }
        }
#endif

        sf::Event event {};
        while (m_sfmlWindow.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(m_sfmlWindow, event);

            switch (event.type) {
                case sf::Event::Closed:
                    if (!dispatchEvent<b::Events::WindowCloseEvent>()) {
                        m_sfmlWindow.close();
                    }
                    break;

                case sf::Event::Resized:
                    m_sfmlWindow.setView(sf::View(sf::FloatRect({ 0, 0 }, { static_cast<float>(event.size.width), static_cast<float>(event.size.height) })));
                    dispatchEvent<b::Events::WindowResizeEvent>(event.size.width, event.size.height);
                    break;

                case sf::Event::LostFocus:
                    dispatchEvent<b::Events::WindowLostFocusEvent>();
                    break;

                case sf::Event::GainedFocus:
                    dispatchEvent<b::Events::WindowGainedFocusEvent>();
                    break;

                case sf::Event::KeyPressed:
                    dispatchEvent<b::Events::KeyPressEvent>(event.key);
                    break;

                case sf::Event::KeyReleased:
                    dispatchEvent<b::Events::KeyReleaseEvent>(event.key);
                    break;

                case sf::Event::TextEntered:
                    dispatchEvent<b::Events::TextEnteredEvent>(event.text);
                    break;

                case sf::Event::MouseWheelScrolled:
                    dispatchEvent<b::Events::MouseWheelScrollEvent>(event.mouseWheelScroll);
                    break;

                case sf::Event::MouseButtonPressed:
                    dispatchEvent<b::Events::MouseButtonPressEvent>(event.mouseButton);
                    break;

                case sf::Event::MouseButtonReleased:
                    dispatchEvent<b::Events::MouseButtonReleaseEvent>(event.mouseButton);
                    break;

                case sf::Event::MouseMoved:
                    m_mousePos = { static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y) };
                    dispatchEvent<b::Events::MouseMoveEvent>(event.mouseMove);
                    break;

                case sf::Event::MouseEntered:
                    dispatchEvent<b::Events::MouseEnteredWindowEvent>();
                    break;

                case sf::Event::MouseLeft:
                    dispatchEvent<b::Events::MouseLeftWindowEvent>();
                    break;

                case sf::Event::JoystickButtonPressed:
                    dispatchEvent<b::Events::JoystickButtonPressEvent>(event.joystickButton);
                    break;

                case sf::Event::JoystickButtonReleased:
                    dispatchEvent<b::Events::JoystickButtonReleaseEvent>(event.joystickButton);
                    break;

                case sf::Event::JoystickMoved:
                    dispatchEvent<b::Events::JoystickMoveEvent>(event.joystickMove);
                    break;

                case sf::Event::JoystickConnected:
                    dispatchEvent<b::Events::JoystickConnectEvent>(event.joystickConnect);
                    break;

                case sf::Event::JoystickDisconnected:
                    dispatchEvent<b::Events::JoystickDisconnectEvent>(event.joystickConnect);
                    break;

                case sf::Event::TouchBegan:
                    dispatchEvent<b::Events::TouchBeginEvent>(event.touch);
                    break;

                case sf::Event::TouchMoved:
                    dispatchEvent<b::Events::TouchMoveEvent>(event.touch);
                    break;

                case sf::Event::TouchEnded:
                    dispatchEvent<b::Events::TouchEndEvent>(event.touch);
                    break;

                case sf::Event::SensorChanged:
                    dispatchEvent<b::Events::SensorChangeEvent>(event.sensor);
                    break;

                default:
                    break;
            }

        }

        m_mouseDelta = m_mousePos - m_mousePosPrev;
        m_mousePosPrev = m_mousePos;

        b::update_themes();

        // Load python if not loaded already
        if (!m_uiScriptResource.string().empty() && !m_uiScriptLoaded) {
            m_uiScriptLoaded = true;
            m_errorMessage = {};
#ifndef B_OS_WINDOWS
            b::log::info("Loading b::window python ui script");
#endif
            try {
                b::py::exec(m_uiScriptResource.string());
            }
            catch (const std::exception &e) {
                ImGui::ErrorCheckEndFrameRecover(nullptr);
                RecoverImguiFontStack();
                m_errorMessage = e.what();
                b::log::error("Unhandled exception:\n{}", e.what());
            }

            b::log::info("Loaded successfully");
        }

        m_sfmlWindow.clear(b::Constants::DefaultWindowBackgroundColor());
        ImGui::SFML::Update(m_sfmlWindow, m_deltaClock.restart());
        b::LockFontStack();

        // And then render
        if (m_errorMessage.has_value()) {
            renderErrorMessage(m_errorMessage.value());    // Py init error
        }
        else {
            try {
                b::PushFont("default");
                style.push();

                onUpdate();

                if (m_pythonUiLoopCallback) {
                    m_pythonUiLoopCallback();
                }

                style.pop();
                b::PopFont();
            }
            catch (const std::exception &e) {
                ImGui::ErrorCheckEndFrameRecover(nullptr);
                RecoverImguiFontStack();
                renderErrorMessage(e.what());     // Py ui loop error
                b::log::error("Unhandled exception:\n{}", e.what());
            }
        }

        b::UnlockFontStack();
        ImGui::SFML::Render(m_sfmlWindow);
        m_sfmlWindow.display();

        if (isOpen()) {
            m_lastWindowState.position = getPosition();
            m_lastWindowState.size = getSize();
            m_lastWindowState.maximized = isMaximized();
        }
    }

    void BaseWindow::renderErrorMessage(const b::string& error) {
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
        m_errorTextWidget.alignv = b::AlignV::Top;
        m_errorTextWidget.style["ImGuiCol_Text"] = "#D34040"_u;

        m_errorPanelWidget([this]() {
            m_errorTextWidget();
        });
    }

}