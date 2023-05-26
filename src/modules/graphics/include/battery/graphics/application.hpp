#pragma once

#include "battery/core/console_application.hpp"
#include "battery/graphics/window.hpp"
#include "battery/python/python.hpp"

namespace b {

    class application : public b::console_application {
    public:
        application();
        virtual ~application() = default;

        static application & get();

        virtual void setup() = 0;
        virtual void update() = 0;
        virtual void cleanup() = 0;

        std::vector<std::shared_ptr<b::window>> windows;

    private:
        void console_setup() final override;
        void console_update() final override;
        void console_cleanup() final override;

        b::py::scoped_interpreter guard{};
    };

}