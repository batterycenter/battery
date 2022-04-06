#pragma once

#include "Battery/Core/Config.h"
#include "cstdint"

namespace Battery {
	namespace MemoryUtils {
#ifdef BATTERY_DEBUG // TODO

		int64_t GetTotalNumberOfBytesAllocated();
		uint64_t GetTotalNumberOfAllocations();

#endif 
	}
}
