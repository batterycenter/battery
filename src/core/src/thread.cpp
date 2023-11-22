
#include "battery/thread.hpp"
#include "battery/application.hpp"

namespace b {

    void thread::closeApplicationIfExists() {
        if (b::Application::instanceExists()) {
            b::Application::get().close();
        }
    }

} // namespace b