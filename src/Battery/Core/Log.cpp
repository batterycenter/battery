
#include "Battery/Core/Log.h"

namespace Battery::Log {

    namespace Internal {
        std::shared_ptr<spdlog::logger> core_logger;
        std::shared_ptr<spdlog::logger> user_logger;
    }

}
