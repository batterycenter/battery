
#include "battery/core/string.hpp"
#include "battery/graphics/application.hpp"
#include "battery/graphics/font_stack.hpp"

namespace b {

    application::application() {
        set_requested_framerate(60.0);
    }

    application* application::get() {
        return dynamic_cast<application*>(b::console_application::get());
    }

    void application::console_setup() {

        setup();

        for (auto& window : windows) {
            window->setup();
        }
    }

    void application::console_update() {

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

    void application::console_cleanup() {

        cleanup();

        for (auto& window : windows) {
            window->cleanup();
        }

        windows.clear();

        ImGui::SFML::Shutdown();
    }

    void application::register_window(std::shared_ptr<b::window> window) {
        windows.push_back(window);
    }

}
