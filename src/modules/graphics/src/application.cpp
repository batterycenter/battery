
#include "battery/core/string.hpp"
#include "battery/graphics/application.hpp"
#include "battery/graphics/font_stack.hpp"

namespace b {

    basic_application::basic_application() {
        setRequestedFramerate(60.0);
    }

    basic_application & basic_application::get() {
        return dynamic_cast<basic_application&>(b::console_application::get());
    }

    void basic_application::detachWindows() {
        for (auto& window : m_windows) {
            window->detach();
        }
        m_windows.clear();
    }

    std::vector<std::shared_ptr<b::basic_window>>& basic_application::windows() {
        return m_windows;
    }
    void basic_application::consoleSetup() {
            setup();
    }

    void basic_application::consoleUpdate() {

            update();

            for (auto& window : m_windows) {
                window->m_framecount = m_framecount;
                window->m_frametime = m_frametime;
                window->m_framerate = m_framerate;
                window->invoke_update();
            }

            bool should_stop = true;
            for (auto& window : m_windows) {
                window->invoke_update();
                if (window->m_sfmlWindow.isOpen()) {
                    should_stop = false;
                }
            }

            if (should_stop) {
                this->stopApplication();
            }
    }

    void basic_application::consoleCleanup() {
            cleanup();
            detachWindows();

            ImGui::SFML::Shutdown();
    }

}
