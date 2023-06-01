
#include "battery/core/string.hpp"
#include "battery/graphics/application.hpp"
#include "battery/graphics/font_stack.hpp"

namespace b {

    application::application() {
        setRequestedFramerate(60.0);
    }

    application & application::get() {
        return dynamic_cast<application&>(b::console_application::get());
    }

    void application::detachWindows() {
        for (auto& window : m_windows) {
            window->detach();
        }
        m_windows.clear();
    }

    std::vector<std::shared_ptr<b::window>>& application::windows() {
        return m_windows;
    }

    void application::consoleSetup() {
        setup();
    }

    void application::consoleUpdate() {

        update();

        for (auto& window : m_windows) {
            window->m_framecount = m_framecount;
            window->m_frametime = m_frametime;
            window->m_framerate = m_framerate;
            window->_update();
        }

        bool should_stop = true;
        for (auto& window : m_windows) {
            window->_update();
            if (window->m_sfmlWindow.isOpen()) {
                should_stop = false;
            }
        }

        if (should_stop) {
            this->stopApplication();
        }
    }

    void application::consoleCleanup() {
        cleanup();
        detachWindows();

        ImGui::SFML::Shutdown();
    }

}
