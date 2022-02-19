
#include "Battery/pch.h"
#include "Battery/Log/Log.h"

#ifdef BATTERY_LOGGING

namespace Battery {
	namespace Log {

		std::shared_ptr<spdlog::logger> coreLogger;
		std::shared_ptr<spdlog::logger> clientLogger;

	}
}

#endif
