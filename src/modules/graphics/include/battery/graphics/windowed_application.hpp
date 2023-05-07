#pragma once

#include "battery/core/application.hpp"
#include "battery/graphics/window.hpp"

namespace b {

    class windowed_application : public b::application {
    public:
        windowed_application();
        virtual ~windowed_application() = default;

        void app_setup() final override;
        void app_update() final override;
        void app_cleanup() final override;

        void register_window(std::shared_ptr<b::window> window);
        [[nodiscard]] std::vector<std::shared_ptr<b::window>> get_windows() const { return windows; }
        [[nodiscard]] std::shared_ptr<b::window> get_window(size_t index) const { return windows[index]; }

        virtual void setup() = 0;
        virtual void update() = 0;
        virtual void cleanup() = 0;

    private:
        std::vector<std::shared_ptr<b::window>> windows;
    };

}