
#ifndef B_OS_WEB

#include "battery/thread.hpp"
#include "battery/application.hpp"

namespace b {

    void thread::closeApplicationIfExists() {
        b::log::error("thread::closeApplicationIfExists() not implemented");
        // if (b::Application::instanceExists()) {
        //     b::Application::get().close();
        // }
    }

} // namespace b

#endif // !B_OS_WEB
