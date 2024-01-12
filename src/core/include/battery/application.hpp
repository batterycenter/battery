#pragma once

#include <vector>
#include <functional>
#include <map>
#include "string.hpp"
#include "time.hpp"
#include "eventbus.hpp"
#include "events.hpp"
#include "renderer.hpp"

extern std::unique_ptr<b::Application> CreateApp();

namespace b {

    class Application {
    public:
        std::unique_ptr<RenderWindow> window;

        Application();
        virtual ~Application();

        static Application& get();

        [[nodiscard]] const std::vector<std::string>& args() const;
        [[nodiscard]] double framerate() const;
        [[nodiscard]] double frametime() const;
        [[nodiscard]] uint64_t framecount() const;
        [[nodiscard]] int exitCode() const;

        virtual void onSetup() = 0;
        virtual void onUpdate() = 0;
        virtual void onRender() = 0;
        virtual void onClose() = 0;

        void setRequestedFramerate(double requestedFramerate);
        void setRequestedFrametime(double requestedFrametime);
        void setExitCode(int exitCode);

        void close();
        void setCloseRequested(bool closeRequested);

        void requestAnimationFrameIn(double seconds);

        void runSingleFrame();
        [[nodiscard]] int run(int argc, char* argv[]);

        template<typename T, typename... TArgs>
        auto attachEventHandler(TArgs&&... args) {
            return m_eventbus.attachEventHandler<T>(std::forward<TArgs>(args)...);
        }

        template<typename T, typename... TArgs>
        auto dispatchEvent(TArgs&&... args) {
            return m_eventbus.dispatchEvent<T>(std::forward<TArgs>(args)...);
        }

        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;
        Application(const Application&) = delete;
        Application(Application&&) = delete;

    private:

        void preUpdate();
        void postUpdate();

        bool callSetup();
        bool callUpdate();
        bool callRender();
        bool callClose();
        void processEvent(SDL_Event& event);
        void waitEvent();

        bool m_closeRequested { false };
        double m_requestedFrametime { 1.0 / 60.0 };
        std::optional<double> m_requestAnimationFrameIn {};

        b::EventBus m_eventbus;

        double m_lastFrame = b::time();
        double m_actualFramerate { 0.0 };
        double m_actualFrametime { 0.0 };
        std::vector<std::string> m_args;
        uint64_t m_framecount { 0 };
        int m_exitCode { 0 };
    };

}
