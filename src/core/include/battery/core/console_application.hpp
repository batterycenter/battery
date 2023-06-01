#pragma once

#include <vector>
#include <functional>
#include <map>
#include "battery/core/string.hpp"

namespace b {

    class console_application {
    public:
        console_application();
        virtual ~console_application() = default;

        static console_application & get();
        static bool instanceExists();

        std::vector<b::string> m_args;
        double m_framerate { 0.0 };
        double m_frametime { 0.0 };
        uint64_t m_framecount { 0 };
        int m_exitCode { 0 };

        virtual void consoleSetup();
        virtual void consoleUpdate();
        virtual void consoleCleanup();

        void setRequestedFramerate(double requested_framerate);
        void setRequestedFrametime(double requested_frametime);

        void stopApplication();
        void setStopRequested(bool stop_requested);

        [[nodiscard]] int run(const b::string& appname, const std::vector<b::string>& args);

        console_application& operator=(const console_application&) = delete;
        console_application& operator=(console_application&&) = delete;
        console_application(const console_application&) = delete;
        console_application(console_application&&) = delete;

    private:

        bool _setup();
        bool _update();
        bool _cleanup();

        bool m_stopRequested { false };
        double m_requestedFramerate { 0.0 };
        inline static console_application* m_instance { nullptr };
    };

}