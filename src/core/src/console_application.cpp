
#include <utility>

#include "battery/core/console_application.hpp"
#include "battery/core/folders.hpp"
#include "battery/core/log.hpp"
#include "battery/core/time.hpp"

namespace b {

    ConsoleApplication::ConsoleApplication() {
        if (m_instance != nullptr) {
            throw std::runtime_error("Only one instance of b::console_application or b::application is ever allowed to exist");
        }
        m_instance = this;
    }

    ConsoleApplication & ConsoleApplication::get() {
        if (m_instance == nullptr) {
            throw std::runtime_error("No instance of b::console_application or b::application exists");
        }
        return *m_instance;
    }

    bool ConsoleApplication::instanceExists() {
        return m_instance != nullptr;
    }

    void ConsoleApplication::onConsoleSetup() {
        // No action
    }

    void ConsoleApplication::onConsoleUpdate() {
        // No action
    }

    void ConsoleApplication::onConsoleExit() {
        // No action
    }




    void ConsoleApplication::setRequestedFramerate(double requested_framerate) {
        this->m_requestedFramerate = requested_framerate;
    }

    void ConsoleApplication::setRequestedFrametime(double requested_frametime) {
        this->m_requestedFramerate = 1.0 / requested_frametime;
    }

    void ConsoleApplication::stopApplication() {
        this->m_stopRequested = true;
    }

    void ConsoleApplication::setStopRequested(bool stop_requested) {
        this->m_stopRequested = stop_requested;
    }




    int ConsoleApplication::run(const b::string& appname, const std::vector<b::string>& args) {
        this->args = args;
        b::Folders::ApplicationName() = appname;

        if (!this->_setup()) {
            this->_exit();
            return m_exitCode;
        }

        this->frametime = 0.0;
        this->framerate = 0.0;

        auto lastFrame = b::time();
        while (!this->m_stopRequested) {

            if (!this->_update()) {
                stopApplication();
            }
            sleep(1.0 / this->m_requestedFramerate - (b::time() - lastFrame));

            this->framecount++;
            this->frametime = b::time() - lastFrame;
            if (this->frametime > 0.0) {
                this->framerate = 1.0 / this->frametime;
            }

            lastFrame = b::time();
        }
        _exit();
        return exitCode;
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

    bool ConsoleApplication::_setup() {
        return CatchExceptions("b::application::setup()", &ConsoleApplication::onConsoleSetup, this);
    }

    bool ConsoleApplication::_update() {
        return CatchExceptions("b::application::update()", &ConsoleApplication::onConsoleUpdate, this);
    }

    bool ConsoleApplication::_exit() {
        return CatchExceptions("b::application::exit()", &ConsoleApplication::onConsoleExit, this);
    }

} // namespace b