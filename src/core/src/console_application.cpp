
#include <utility>

#include "battery/core/console_application.hpp"
#include "battery/core/folders.hpp"
#include "battery/core/log.hpp"
#include "battery/core/time.hpp"

namespace b {

    console_application::console_application() {
        if (m_instance != nullptr) {
            throw std::runtime_error("Only one instance of b::console_application or b::application is ever allowed to exist");
        }
        m_instance = this;
    }

    console_application & console_application::get() {
        if (m_instance == nullptr) {
            throw std::runtime_error("No instance of b::console_application or b::application exists");
        }
        return *m_instance;
    }

    bool console_application::instanceExists() {
        return m_instance != nullptr;
    }

    void console_application::consoleSetup() {
        // No action
    }

    void console_application::consoleUpdate() {
        // No action
    }

    void console_application::consoleCleanup() {
        // No action
    }




    void console_application::setRequestedFramerate(double requested_framerate) {
        this->m_requestedFramerate = requested_framerate;
    }

    void console_application::setRequestedFrametime(double requested_frametime) {
        this->m_requestedFramerate = 1.0 / requested_frametime;
    }

    void console_application::stopApplication() {
        this->m_stopRequested = true;
    }

    void console_application::setStopRequested(bool stop_requested) {
        this->m_stopRequested = stop_requested;
    }




    int console_application::run(const b::string& appname, const std::vector<b::string>& args) {
        this->m_args = args;
        b::Folders::ApplicationName() = appname;

        if (!this->_setup()) {
            this->_cleanup();
            return m_exitCode;
        }

        this->m_frametime = 0.0;
        this->m_framerate = 0.0;

        auto lastFrame = b::time();
        while (!this->m_stopRequested) {

            if (!this->_update()) {
                stopApplication();
            }
            sleep(1.0 / this->m_requestedFramerate - (b::time() - lastFrame));

            this->m_framecount++;
            this->m_frametime = b::time() - lastFrame;
            if (this->m_frametime > 0.0) {
                this->m_framerate = 1.0 / this->m_frametime;
            }

            lastFrame = b::time();
        }
        this->_cleanup();
        return m_exitCode;
    }



    template<typename Fn, typename... Args>
    bool CatchExceptions(const b::string& funcname, Fn&& func, Args&&... args) {
        try {
            std::invoke(std::forward<Fn>(func), std::forward<Args>(args)...);
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
        return CatchExceptions("b::application::setup()", &console_application::consoleSetup, this);
    }

    bool console_application::_update() {
        return CatchExceptions("b::application::update()", &console_application::consoleUpdate, this);
    }

    bool console_application::_cleanup() {
        return CatchExceptions("b::application::cleanup()", &console_application::consoleCleanup, this);
    }

} // namespace b