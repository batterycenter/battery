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

        std::vector<std::string> args;
        double framerate { 0.0 };
        double frametime { 0.0 };
        uint64_t framecount { 0 };

        virtual void setup();
        virtual void update();
        virtual void cleanup();

        void set_requested_framerate(double requested_framerate);
        void set_requested_frametime(double requested_frametime);

        void stop_application();
        void set_stop_requested(bool stop_requested);

        void run(const std::string& appname, const std::vector<std::string>& args);

    private:

        void _setup();
        void _update();
        void _cleanup();

        bool stop_requested { false };
        double requested_framerate { 0.0 };
    };

}