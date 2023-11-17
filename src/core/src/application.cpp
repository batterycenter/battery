
#include <utility>

#include "battery/application.hpp"
#include "battery/folders.hpp"
#include "battery/log.hpp"
#include "battery/time.hpp"
#include "battery/window.hpp"

#include "SDL2/SDL.h"
#include "SDL_image.h"

namespace b {

    Application::Application() {
        if (m_instance != nullptr) {
            throw std::runtime_error("Only one instance of b::console_application "
                                     "or b::application is ever allowed to exist");
        }
        m_instance = this;

        SDL_Init(SDL_INIT_EVERYTHING);
        IMG_Init(IMG_INIT_PNG);
    }

    Application::~Application() {
        IMG_Quit();
        SDL_Quit();
        m_instance = nullptr;
    }

    Application& Application::get() {
        if (m_instance == nullptr) {
            throw std::runtime_error("No instance of b::console_application or b::application exists");
        }
        return *m_instance;
    }

    bool Application::instanceExists() {
        return m_instance != nullptr;
    }

    const std::vector<std::string>& Application::args() const {
        return this->m_args;
    }

    double Application::framerate() const {
        return this->m_actualFramerate;
    }

    double Application::frametime() const {
        return this->m_actualFrametime;
    }

    uint64_t Application::framecount() const {
        return this->m_framecount;
    }

    int Application::exitCode() const {
        return this->m_exitCode;
    }

    void Application::onSetup() {
        // No default action
    }

    void Application::onUpdate() {
        // No default action
    }

    void Application::onClose() {
        // No default action
    }




    void Application::setRequestedFramerate(double requestedFramerate) {
        m_requestedFrametime = 1.0 / requestedFramerate;
    }

    void Application::setRequestedFrametime(double requestedFrametime) {
        m_requestedFrametime = requestedFrametime;
    }

    void Application::setExitCode(int exitCode) {
        m_exitCode = exitCode;
    }

    void Application::close() {
        this->m_closeRequested = true;
    }

    void Application::setCloseRequested(bool closeRequested) {
        this->m_closeRequested = closeRequested;
    }




    int Application::run(const std::string& appname, int argc, const char** argv) {
        b::Folders::ApplicationName() = appname;

        m_args.clear();
        m_args.reserve(argc);
        for (int i = 0; i < argc; i++) {
            m_args.emplace_back(argv[i]);
        }

        if (!callSetup()) {
            close();
        }

        auto lastFrame = b::time();
        while (!this->m_closeRequested) {

            preUpdate();

            if (!this->callUpdate()) {
                close();
            }
            sleep(m_requestedFrametime - (b::time() - lastFrame));
            m_actualFrametime = b::time() - lastFrame;
            lastFrame = b::time();

            postUpdate();
        }

        callClose();
        return m_exitCode;
    }



    template<typename Fn, typename... Args>
    bool CatchExceptions(const std::string& funcname, Fn&& func, Args&&... args) {
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

    void Application::preUpdate() {
        pollEvents();
    }

    void Application::postUpdate() {
        if (m_actualFrametime > 0.0) {
            m_actualFramerate = 1.0 / m_actualFrametime;
        }
        m_framecount++;
    }

    bool Application::callSetup() {
        return CatchExceptions("b::application::setup()", &Application::onSetup, this);
    }

    bool Application::callUpdate() {
        return CatchExceptions("b::application::update()", &Application::onUpdate, this);
    }

    bool Application::callClose() {
        return CatchExceptions("b::application::exit()", &Application::onClose, this);
    }

    void Application::pollEvents() {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            auto idValid = Window::m_windowIDs.contains(event.window.windowID);
            auto getWindow = [&]() -> Window& {
                if (!idValid) {
                    throw b::impossible_situation_error("[Application::pollEvents] Window ID is invalid");
                }
                return Window::m_windowIDs.at(event.window.windowID);
            };

            switch (event.type) {

                // This is the global quit: When the last window was closed, including SIGINT
                case SDL_QUIT:
                    if (!dispatchEvent<ApplicationQuitEvent>()) {
                        close();
                    }
                    if (idValid) {
                        auto& window = getWindow();
                        if (!window.dispatchEvent<WindowCloseEvent>()) {
                            window.close();
                        }
                    }
                    break;

                // Close event but only per window
                case SDL_WINDOWEVENT_CLOSE:
                    if (idValid) {
                        auto& window = getWindow();
                        if (!window.dispatchEvent<WindowCloseEvent>()) {
                            window.close();
                        }
                    }
                    break;

                case SDL_WINDOWEVENT_RESIZED:
                    if (idValid) {
                        getWindow().dispatchEvent<WindowResizeEvent>(
                                b::Vec2(event.window.data1, event.window.data2)
                            );
                    }
                    break;

                case SDL_WINDOWEVENT_FOCUS_LOST:
                    if (idValid) {
                        getWindow().dispatchEvent<WindowLostFocusEvent>();
                    }
                    break;

                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    if (idValid) {
                        getWindow().dispatchEvent<WindowGainedFocusEvent>();
                    }
                    break;

                case SDL_WINDOWEVENT_MAXIMIZED:
                    if (idValid) {
                        getWindow().dispatchEvent<WindowMaximizedEvent>();
                    }
                    break;

                case SDL_WINDOWEVENT_MINIMIZED:
                    if (idValid) {
                        getWindow().dispatchEvent<WindowMinimizedEvent>();
                    }
                    break;

                case SDL_WINDOWEVENT_RESTORED:
                    if (idValid) {
                        getWindow().dispatchEvent<WindowRestoredEvent>();
                    }
                    break;

                case SDL_WINDOWEVENT_MOVED:
                    if (idValid) {
                        getWindow().dispatchEvent<WindowMovedEvent>(
                                b::Vec2(event.window.data1, event.window.data2)
                            );
                    }
                    break;

                case SDL_WINDOWEVENT_HIDDEN:
                    if (idValid) {
                        getWindow().dispatchEvent<WindowHiddenEvent>();
                    }
                    break;

                case SDL_WINDOWEVENT_SHOWN:
                    if (idValid) {
                        getWindow().dispatchEvent<WindowShownEvent>();
                    }
                    break;

                case SDL_WINDOWEVENT_EXPOSED:
                    if (idValid) {
                        getWindow().dispatchEvent<WindowExposedEvent>();
                    }
                    break;

                case SDL_WINDOWEVENT_ENTER:
                    if (idValid) {
                        getWindow().dispatchEvent<MouseEnteredWindowEvent>();
                    }
                    break;

                case SDL_WINDOWEVENT_LEAVE:
                    if (idValid) {
                        getWindow().dispatchEvent<MouseLeftWindowEvent>();
                    }
                    break;

//                case sf::Event::MouseWheelScrolled:
//                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
//                        mouse.setScrollDelta(b::Vec2(0, event.mouseWheelScroll.delta));
//                    }
//                    else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
//                        mouse.setScrollDelta(b::Vec2(event.mouseWheelScroll.delta, 0));
//                    }
//                    dispatchEvent<b::Events::MouseWheelScrollEvent>(mouse.scrollDelta);
//                    break;
//
//                case sf::Event::MouseButtonPressed:
//                    dispatchEvent<b::Events::MouseButtonPressEvent>(event.mouseButton);
//                    break;
//
//                case sf::Event::MouseButtonReleased:
//                    dispatchEvent<b::Events::MouseButtonReleaseEvent>(event.mouseButton);
//                    break;
//
//                case sf::Event::MouseMoved:
//                    m_mouseMoveEventData.updatePositionData(b::Vec2(event.mouseMove.x, event.mouseMove.y));
//                    dispatchEvent<b::Events::MouseMoveEvent>(m_mouseMoveEventData);
//                    break;
//
//                case sf::Event::MouseEntered:
//                    dispatchEvent<b::Events::MouseEnteredWindowEvent>();
//                    break;
//
//                case sf::Event::MouseLeft:
//                    dispatchEvent<b::Events::MouseLeftWindowEvent>();
//                    break;
//
//                case sf::Event::JoystickButtonPressed:
//                    dispatchEvent<b::Events::JoystickButtonPressEvent>(event.joystickButton);
//                    break;
//
//                case sf::Event::JoystickButtonReleased:
//                    dispatchEvent<b::Events::JoystickButtonReleaseEvent>(event.joystickButton);
//                    break;
//
//                case sf::Event::JoystickMoved:
//                    dispatchEvent<b::Events::JoystickMoveEvent>(event.joystickMove);
//                    break;
//
//                case sf::Event::JoystickConnected:
//                    dispatchEvent<b::Events::JoystickConnectEvent>(event.joystickConnect);
//                    break;
//
//                case sf::Event::JoystickDisconnected:
//                    dispatchEvent<b::Events::JoystickDisconnectEvent>(event.joystickConnect);
//                    break;
//
//                case sf::Event::TouchBegan:
//                    dispatchEvent<b::Events::TouchBeginEvent>(event.touch);
//                    break;
//
//                case sf::Event::TouchMoved:
//                    dispatchEvent<b::Events::TouchMoveEvent>(event.touch);
//                    break;
//
//                case sf::Event::TouchEnded:
//                    dispatchEvent<b::Events::TouchEndEvent>(event.touch);
//                    break;
//
//                case sf::Event::SensorChanged:
//                    dispatchEvent<b::Events::SensorChangeEvent>(event.sensor);
//                    break;

                default:
                    break;
            }

        }
    }

} // namespace b