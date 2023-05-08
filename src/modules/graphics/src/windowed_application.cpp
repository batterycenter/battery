
#include "battery/core/string.hpp"
#include "battery/graphics/windowed_application.hpp"
#include "battery/graphics/font_stack.hpp"

namespace b {

    windowed_application::windowed_application() {
        set_requested_framerate(60.0);
    }

    void windowed_application::app_setup() {

        setup();

        for (auto& window : windows) {
            window->setup();
        }
    }

    void windowed_application::app_update() {

        update();

        for (auto& window : windows) {
            window->framecount = framecount;
            window->frametime = frametime;
            window->framerate = framerate;
            window->_update();
        }

        bool should_stop = true;
        for (auto& window : windows) {
            window->_update();
            if (window->getWindow().isOpen()) {
                should_stop = false;
            }
        }

        if (should_stop) {
            this->stop_application();
        }
    }

    void windowed_application::app_cleanup() {

        cleanup();

        for (auto& window : windows) {
            window->cleanup();
        }

        windows.clear();

        ImGui::SFML::Shutdown();
    }

    void windowed_application::register_window(std::shared_ptr<b::window> window) {
        windows.push_back(window);
    }

}
