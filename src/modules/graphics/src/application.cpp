
#include "battery/core/string.hpp"
#include "battery/graphics/application.hpp"
#include "battery/graphics/font_stack.hpp"

namespace b {

    Application::Application() {
        setRequestedFramerate(60.0);
    }

    Application::~Application() {
        detachWindows();
        ImGui::SFML::Shutdown();
    }

    Application & Application::get() {
        return dynamic_cast<Application&>(b::ConsoleApplication::get());
    }

    void Application::detachWindows() {
        while (!m_windows.empty()) {
            detachWindow(m_windows.back().get());
        }
    }

    b::Window& Application::getCurrentlyUpdatingWindow() {
        if (m_currentActiveWindow == nullptr) {
            throw std::runtime_error("Application::getCurrentlyUpdatingWindow(): Cannot get window, no window is currently updating or rendering");
        }
        return *m_currentActiveWindow;
    }

    std::vector<std::reference_wrapper<b::Window>>& Application::windowRefs() {
        return m_windows;
    }

    void Application::onConsoleSetup() {
        onSetup();
    }

    void Application::onConsoleUpdate() {
        onUpdate();
        for (auto& window : m_windows) {
            window.get().framecount = framecount;
            window.get().frametime = frametime;
            window.get().framerate = framerate;
            m_currentActiveWindow = &window.get();
            window.get().invokeUpdate();
        }
        m_currentActiveWindow = nullptr;

        onRender();
        for (auto& window : m_windows) {
            m_currentActiveWindow = &window.get();
            window.get().invokeRender();
        }
        m_currentActiveWindow = nullptr;
    }

    void Application::onConsoleExit() {
        onExit();
        detachWindows();
    }

} // namespace b