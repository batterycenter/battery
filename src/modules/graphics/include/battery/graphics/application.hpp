#pragma once

#include "battery/core/console_application.hpp"
#include "battery/graphics/window.hpp"
#include "battery/python/python.hpp"

namespace b {

    class application : public b::console_application {
    public:
        application();
        virtual ~application() = default;

        static application* get();

        void console_setup() final override;
        void console_update() final override;
        void console_cleanup() final override;

        void register_window(std::shared_ptr<b::window> window);
        [[nodiscard]] std::vector<std::shared_ptr<b::window>> get_windows() const { return windows; }
        [[nodiscard]] std::shared_ptr<b::window> get_window(size_t index) const { return windows[index]; }

        virtual void setup() = 0;
        virtual void update() = 0;
        virtual void cleanup() = 0;

    private:
        b::py::scoped_interpreter guard{};
        std::vector<std::shared_ptr<b::window>> windows;
    };

}