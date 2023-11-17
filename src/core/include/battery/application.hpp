#pragma once

#include <vector>
#include <functional>
#include <map>
#include "string.hpp"
#include "eventbus.hpp"
#include "events.hpp"

namespace b {

    class Application {
    public:
        Application();
        virtual ~Application();

        static Application& get();
        static bool instanceExists();

        const std::vector<std::string>& args() const;
        double framerate() const;
        double frametime() const;
        uint64_t framecount() const;
        int exitCode() const;

        virtual void onSetup() = 0;
        virtual void onUpdate() = 0;
        virtual void onClose() = 0;

        void setRequestedFramerate(double requestedFramerate);
        void setRequestedFrametime(double requestedFrametime);
        void setExitCode(int exitCode);

        void close();
        void setCloseRequested(bool closeRequested);

        [[nodiscard]] int run(const std::string& appname, int argc, const char** argv);

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
        bool callClose();
        void pollEvents();

        bool m_closeRequested { false };
        double m_requestedFrametime { 1.0 / 60.0 };

        b::EventBus m_eventbus;

        std::vector<std::string> m_args;
        double m_actualFramerate { 0.0 };
        double m_actualFrametime { 0.0 };
        uint64_t m_framecount { 0 };
        int m_exitCode { 0 };

        inline static Application* m_instance { nullptr };
    };

}