
#include <utility>

#include "battery/core/application.hpp"
#include "battery/core/time.hpp"
#include "battery/core/log.hpp"

namespace b {

    void application::setup() {
        // No action
    }

    void application::update() {
        // No action
    }

    void application::cleanup() {
        // No action
    }




    void application::set_requested_framerate(double requested_framerate) {
        this->requested_framerate = requested_framerate;
    }

    void application::set_requested_frametime(double requested_frametime) {
        this->requested_framerate = 1.0 / requested_frametime;
    }

    void application::stop_application() {
        this->stop_requested = true;
    }

    void application::set_stop_requested(bool stop_requested) {
        this->stop_requested = stop_requested;
    }




    void application::run(const std::vector<std::string>& args) {
        this->args = args;
        this->_setup();

        this->frametime = 0.0;
        this->framerate = 0.0;

        auto last_frame = b::time();
        while (!this->stop_requested) {

            this->_update();
            sleep(1.0 / this->requested_framerate - (b::time() - last_frame));

            this->framecount++;
            this->frametime = b::time() - last_frame;
            if (this->frametime > 0.0) {
                this->framerate = 1.0 / this->frametime;
            }

            last_frame = b::time();
        }
        this->_cleanup();
    }



    template<typename Fn, typename... Args>
    void catch_exceptions(const std::string& funcname, Fn&& fn, Args&&... args) {
        try {
            std::invoke(std::forward<Fn>(fn), std::forward<Args>(args)...);
        }
        catch (const std::exception& e) {
            b::log::critical("Unhandled exception caught in {}: {}", funcname, e.what());
        }
        catch (...) {
            b::log::critical("Unknown exception caught in {}! No more information available", funcname);
        }
    }

    void application::_setup() {
        catch_exceptions("b::application::setup()", &application::setup, this);
    }

    void application::_update() {
        catch_exceptions("b::application::update()", &application::update, this);
    }

    void application::_cleanup() {
        catch_exceptions("b::application::cleanup()", &application::cleanup, this);
    }

}