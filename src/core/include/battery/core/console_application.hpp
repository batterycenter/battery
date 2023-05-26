#pragma once

#include <vector>
#include <string>
#include <functional>
#include <map>

namespace b {

    class console_application {
    public:
        console_application();
        virtual ~console_application() = default;

        static console_application* get();

        std::vector<std::u8string> args;
        double framerate { 0.0 };
        double frametime { 0.0 };
        uint64_t framecount { 0 };

        virtual void console_setup();
        virtual void console_update();
        virtual void console_cleanup();

        void set_requested_framerate(double requested_framerate);
        void set_requested_frametime(double requested_frametime);

        void stop_application();
        void set_stop_requested(bool stop_requested);

        void run(const std::u8string& appname, const std::vector<std::u8string>& args);

        console_application& operator=(const console_application&) = delete;
        console_application& operator=(console_application&&) = delete;
        console_application(const console_application&) = delete;
        console_application(console_application&&) = delete;

    private:

        bool _setup();
        bool _update();
        bool _cleanup();

        bool stop_requested { false };
        double requested_framerate { 0.0 };
        inline static console_application* instance {nullptr };
    };

}