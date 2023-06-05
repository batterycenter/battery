
#include "battery/core/string.hpp"
#include "battery/graphics/application.hpp"
#include "battery/graphics/font_stack.hpp"

namespace b {

    BaseApplication::BaseApplication() {
        setRequestedFramerate(60.0);
    }

    BaseApplication & BaseApplication::get() {
        return dynamic_cast<BaseApplication&>(b::ConsoleApplication::get());
    }

    void BaseApplication::detachWindows() {
        for (auto& window : m_windows) {
            window->detach();
        }
        m_windows.clear();
    }

    std::vector<std::shared_ptr<b::basic_window>>& BaseApplication::windows() {
        return m_windows;
    }
    void BaseApplication::consoleSetup() {
            setup();
    }

    void BaseApplication::consoleUpdate() {

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

    void BaseApplication::consoleCleanup() {
            cleanup();
            detachWindows();

            ImGui::SFML::Shutdown();
    }

}
