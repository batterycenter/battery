
#include <utility>

#include "battery/core/console_application.hpp"
#include "battery/core/time.hpp"
#include "battery/core/log.hpp"
#include "battery/core/folders.hpp"

namespace b {

    console_application::console_application() {
        if (instance != nullptr) throw std::runtime_error("Only one instance of b::console_application or b::application is ever allowed to exist");
        instance = this;
    }

    console_application & console_application::get() {
        if (instance == nullptr) throw std::runtime_error("No instance of b::console_application or b::application exists");
        return *instance;
    }

    void console_application::console_setup() {
        // No action
    }

    void console_application::console_update() {
        // No action
    }

    void console_application::console_cleanup() {
        // No action
    }




    void console_application::set_requested_framerate(double requested_framerate) {
        this->requested_framerate = requested_framerate;
    }

    void console_application::set_requested_frametime(double requested_frametime) {
        this->requested_framerate = 1.0 / requested_frametime;
    }

    void console_application::stop_application() {
        this->stop_requested = true;
    }

    void console_application::set_stop_requested(bool stop_requested) {
        this->stop_requested = stop_requested;
    }




    void console_application::run(const b::string& appname, const std::vector<b::string>& args) {
        this->args = args;
        b::folders::set_application_name(appname);

        if (!this->_setup()) {
            this->_cleanup();
            return;
        }

        this->frametime = 0.0;
        this->framerate = 0.0;

        auto last_frame = b::time();
        while (!this->stop_requested) {

            if (!this->_update()) {
                stop_application();
            }
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
    bool catch_exceptions(const b::string& funcname, Fn&& fn, Args&&... args) {
        try {
            std::invoke(std::forward<Fn>(fn), std::forward<Args>(args)...);
            return true;
        }
        catch (const std::exception& e) {
            b::log::critical("Unhandled exception caught in {}: {}", funcname, e.what());
        }
        catch (...) {
            b::log::critical("Unknown exception caught in {}! No more information available", funcname);
        }
        return false;
    }

    bool console_application::_setup() {
        return catch_exceptions("b::application::setup()", &console_application::console_setup, this);
    }

    bool console_application::_update() {
        return catch_exceptions("b::application::update()", &console_application::console_update, this);
    }

    bool console_application::_cleanup() {
        return catch_exceptions("b::application::cleanup()", &console_application::console_cleanup, this);
    }

}