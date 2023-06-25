
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

    std::vector<std::reference_wrapper<b::Window>>& Application::windowRefs() {
        return m_windows;
    }

    void Application::onConsoleSetup() {
        onSetup();
    }

    void Application::onConsoleUpdate() {
        onUpdate();
        for (auto& window : m_windows) {
            window.get().framecount = m_framecount;
            window.get().frametime = m_frametime;
            window.get().framerate = m_framerate;
            window.get().invokeUpdate();
        }
    }

    void Application::onConsoleExit() {
        onExit();
        detachWindows();
    }

} // namespace b