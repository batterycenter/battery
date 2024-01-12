
#include <memory>
#include <stdexcept>
#include <utility>

#ifdef B_OS_WEB
#include "emscripten.h"
#endif // B_OS_WEB

#include "battery/application.hpp"
#include "battery/folders.hpp"
#include "battery/log.hpp"
#include "battery/time.hpp"
#include "battery/window.hpp"
#include "battery/thread.hpp"

#include "SDL2/SDL.h"
#include "SDL_image.h"

namespace b {

    Application::Application() {
        if (0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            auto msg = b::format("SDL Failed to initialize: {}", SDL_GetError());
            b::log::critical("{}", msg);
            throw std::runtime_error(msg);
        }
        IMG_Init(IMG_INIT_PNG);
    }

    Application::~Application() {
        window.reset();
        IMG_Quit();
        SDL_Quit();
    }

    Application& Application::get() {
        static std::unique_ptr<b::Application> instance = ::CreateApp();
        return *instance;
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

    void Application::requestAnimationFrameIn(double seconds) {
        m_requestAnimationFrameIn = seconds;
    }




    void Application::runSingleFrame() {

        if (!window) {
            return;
        }

        preUpdate();

        if (!this->callUpdate()) {
            close();
        }

        window->prepareFrame();
        if (!this->callRender()) {
            close();
        }
        window->renderFrame();

        sleep(m_requestedFrametime - (b::time() - m_lastFrame));
        m_actualFrametime = b::time() - m_lastFrame;
        if (m_actualFrametime > 0.0) {
            m_actualFramerate = 1.0 / m_actualFrametime;
        }
        m_lastFrame = b::time();

        postUpdate();
    }

    int Application::run(int argc, char* argv[]) {

#ifndef B_PRODUCTION_MODE
        b::log::warn("Battery is not running in production mode, enabling hot-reloaded resources. "
                     "Make sure to enable production mode before releasing your app!");
#endif

        m_args.clear();
        m_args.reserve(argc);
        for (int i = 0; i < argc; i++) {
            m_args.emplace_back(argv[i]);
        }

#ifdef B_OS_WEB
        b::log::info("Setting Emscripten main loop");
        emscripten_set_main_loop([]() {
                b::Application::get().runSingleFrame();
                }, -1, false);
#endif // B_OS_WEB

        window = std::make_unique<b::RenderWindow>(Constants::DefaultWindowTitle(), Constants::DefaultWindowSize());
        if (!callSetup()) {
            close();
        }

#ifdef B_OS_WEB
        return 0;
#else // B_OS_WEB
        while (!this->m_closeRequested) {
            runSingleFrame();
        }
        callClose();
        return m_exitCode;
#endif // B_OS_WEB
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
        waitEvent();
    }

    void Application::postUpdate() {
        m_framecount++;
    }

    bool Application::callSetup() {
        return CatchExceptions("b::application::setup()", &Application::onSetup, this);
    }

    bool Application::callUpdate() {
        return CatchExceptions("b::application::update()", &Application::onUpdate, this);
    }

    bool Application::callRender() {
        return CatchExceptions("b::application::render()", &Application::onRender, this);
    }

    bool Application::callClose() {
        return CatchExceptions("b::application::exit()", &Application::onClose, this);
    }

    void Application::processEvent(SDL_Event& event) {
        if (window) {
            window->processImGuiSDLEvent(&event);
        }

        switch (event.type) {

            // This is the global quit: When the last window was closed, including SIGINT
//                case SDL_QUIT:
//                    if (!dispatchEvent<ApplicationQuitEvent>()) {
//                        close();
//                    }
//                    break;

            case SDL_WINDOWEVENT:
                if (window) {
                    if (event.window.windowID == window->getSDLWindowID()) {
                        window->processSDLWindowEvent(&event);
                    }
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

    void Application::waitEvent() {
        SDL_Event event;

        double delay = Constants::AnimationHeartbeatInterval();
        if (ImGui::GetIO().WantTextInput) {
            delay = std::min(delay, Constants::BlinkingCursorHeartbeatInterval());
        }
        if (m_requestAnimationFrameIn) {
            delay = std::min(delay, *m_requestAnimationFrameIn);
            m_requestAnimationFrameIn = {};
        }

#ifndef B_OS_WEB
        if (0 != SDL_WaitEventTimeout(&event, static_cast<int>(delay * 1000))) {
            processEvent(event);
        }
#else // B_OS_WEB
        while (true) {
            if (0 == SDL_WaitEventTimeout(&event, 0)) {
                break;
            }
            processEvent(event);
        }
#endif // B_OS_WEB
    }

} // namespace b
