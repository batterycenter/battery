
#include "battery/core/log.hpp"

namespace battery::log {

    namespace internal {
        std::shared_ptr<spdlog::logger> core_logger;
        std::shared_ptr<spdlog::logger> user_logger;
    }

}
