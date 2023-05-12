
#include <utility>

#include "battery/core/application.hpp"
#include "battery/core/time.hpp"
#include "battery/core/log.hpp"
#include "battery/core/folders.hpp"

namespace b {

    application::application() {
        if (instance != nullptr) {
            throw std::runtime_error("Only one instance of b::application is allowed!");
        }
        instance = this;
    }

    application* application::get() {
        if (instance == nullptr) {
            throw std::runtime_error("No instance of b::application exists!");
        }
        return instance;
    }

    void application::app_setup() {
        // No action
    }

    void application::app_update() {
        // No action
    }

    void application::app_cleanup() {
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




    void application::run(const std::u8string& appname, const std::vector<std::u8string>& args) {
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
    bool catch_exceptions(const std::string& funcname, Fn&& fn, Args&&... args) {
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

    bool application::_setup() {
        return catch_exceptions("b::application::setup()", &application::app_setup, this);
    }

    bool application::_update() {
        return catch_exceptions("b::application::update()", &application::app_update, this);
    }

    bool application::_cleanup() {
        return catch_exceptions("b::application::cleanup()", &application::app_cleanup, this);
    }

}