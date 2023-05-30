
#include <utility>

#include "battery/core/console_application.hpp"
#include "battery/core/time.hpp"
#include "battery/core/log.hpp"
#include "battery/core/folders.hpp"

namespace b {

    console_application::console_application() {
        if (m_instance != nullptr) throw std::runtime_error("Only one instance of b::console_application or b::application is ever allowed to exist");
        m_instance = this;
    }

    console_application & console_application::get() {
        if (m_instance == nullptr) throw std::runtime_error("No instance of b::console_application or b::application exists");
        return *m_instance;
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
        this->m_requestedFramerate = requested_framerate;
    }

    void console_application::set_requested_frametime(double requested_frametime) {
        this->m_requestedFramerate = 1.0 / requested_frametime;
    }

    void console_application::stop_application() {
        this->m_stopRequested = true;
    }

    void console_application::set_stop_requested(bool stop_requested) {
        this->m_stopRequested = stop_requested;
    }




    int console_application::run(const b::string& appname, const std::vector<b::string>& args) {
        this->m_args = args;
        b::folders::set_application_name(appname);

        if (!this->_setup()) {
            this->_cleanup();
            return m_exitCode;
        }

        this->m_frametime = 0.0;
        this->m_framerate = 0.0;

        auto last_frame = b::time();
        while (!this->m_stopRequested) {

            if (!this->_update()) {
                stop_application();
            }
            sleep(1.0 / this->m_requestedFramerate - (b::time() - last_frame));

            this->m_framecount++;
            this->m_frametime = b::time() - last_frame;
            if (this->m_frametime > 0.0) {
                this->m_framerate = 1.0 / this->m_frametime;
            }

            last_frame = b::time();
        }
        this->_cleanup();
        return m_exitCode;
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