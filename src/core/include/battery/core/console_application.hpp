#pragma once

#include <vector>
#include <functional>
#include <map>
#include "battery/core/string.hpp"

namespace b {

    class ConsoleApplication {
    public:
        ConsoleApplication();
        virtual ~ConsoleApplication() = default;

        static ConsoleApplication & get();
        static bool instanceExists();

        std::vector<std::string> args;
        double framerate { 0.0 };
        double frametime { 0.0 };
        uint64_t framecount { 0 };
        int exitCode { 0 };

        virtual void onConsoleSetup();
        virtual void onConsoleUpdate();
        virtual void onConsoleExit();

        void setRequestedFramerate(double requested_framerate);
        void setRequestedFrametime(double requested_frametime);

        void stopApplication();
        void setStopRequested(bool stop_requested);

        [[nodiscard]] int run(const std::string& appname, const std::vector<std::string>& args);

        ConsoleApplication& operator=(const ConsoleApplication&) = delete;
        ConsoleApplication& operator=(ConsoleApplication&&) = delete;
        ConsoleApplication(const ConsoleApplication&) = delete;
        ConsoleApplication(ConsoleApplication&&) = delete;

    private:

        bool _setup();
        bool _update();
        bool _exit();

        bool m_stopRequested { false };
        double m_requestedFramerate { 0.0 };
        inline static ConsoleApplication* m_instance {nullptr };
    };

}