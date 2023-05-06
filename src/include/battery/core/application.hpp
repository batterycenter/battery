#pragma once

#include <vector>
#include <string>
#include <functional>
#include <map>

namespace b {

    class application {
    public:
        application() = default;
        virtual ~application() = default;

        std::vector<std::u8string> args;
        double framerate { 0.0 };
        double frametime { 0.0 };
        uint64_t framecount { 0 };

        virtual void app_setup();
        virtual void app_update();
        virtual void app_cleanup();

        void set_requested_framerate(double requested_framerate);
        void set_requested_frametime(double requested_frametime);

        void stop_application();
        void set_stop_requested(bool stop_requested);

        void run(const std::u8string& appname, const std::vector<std::u8string>& args);

    private:

        bool _setup();
        bool _update();
        bool _cleanup();

        bool stop_requested { false };
        double requested_framerate { 0.0 };
    };

}