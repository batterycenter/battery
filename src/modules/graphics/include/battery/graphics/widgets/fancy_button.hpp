#pragma once

#include "battery/graphics/widgets/button.hpp"

namespace b::widgets {

    class fancy_button : public b::widgets::button {
    public:
        fancy_button();
        explicit fancy_button(const std::string& name);

        void operator()(const std::function<void()>& callback = nullptr);
    };

}
