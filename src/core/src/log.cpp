
#include "battery/core/log.h"

namespace battery::log {

    namespace internal {
        std::shared_ptr<spdlog::logger> core_logger;
        std::shared_ptr<spdlog::logger> user_logger;
    }

}
