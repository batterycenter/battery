#pragma once

#include <functional>

namespace b {

    void set_ctrl_c_handler(const std::function<void()>& handler);

    namespace internal {
        void init_ctrl_c_handler();
    }

}
