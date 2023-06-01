
#include "battery/core/string.hpp"
#include "battery/graphics/application.hpp"
#include "battery/graphics/font_stack.hpp"

namespace b {

    application::application() {
        set_requested_framerate(60.0);
    }

    application & application::get() {
        return dynamic_cast<application&>(b::console_application::get());
    }

    void application::add_window(std::shared_ptr<b::window> window_ptr) {
        m_windows.push_back(window_ptr);
        m_windows.back()->setup();
    }

    void application::clear_windows() {
        m_windows.clear();
    }

    std::vector<std::shared_ptr<b::window>>& application::windows() {
        return m_windows;
    }

    void application::console_setup() {
        setup();
    }

    void application::console_update() {

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
            this->stop_application();
        }
    }

    void application::console_cleanup() {
        cleanup();
        for (auto& window : m_windows) {
            window->cleanup();
        }
        m_windows.clear();

        ImGui::SFML::Shutdown();
    }

}
