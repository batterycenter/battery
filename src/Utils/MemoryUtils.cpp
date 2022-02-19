
#include "Battery/pch.h"
#include "Battery/Utils/MemoryUtils.h"
#include "Battery/Core/Config.h"
#include "Battery/Log/Log.h"
#include <new>
#include <iostream>
#include <cstdint>

#ifdef BATTERY_DEBUG

namespace Battery {
	namespace MemoryUtils {

		static int64_t totalBytesAllocated = 0;
		static uint64_t totalNumberOfAllocations = 0;

		int64_t GetTotalNumberOfBytesAllocated() {
			return totalBytesAllocated;
		}

		uint64_t GetTotalNumberOfAllocations() {
			return totalNumberOfAllocations;
		}

	}
}


#ifdef BATTERY_ENABLE_ALLOCATION_TRACKING

void* operator new(size_t size) {
	Battery::MemoryUtils::totalBytesAllocated += size;
#ifdef BATTERY_ENABLE_ALLOCATION_PRINTS
	printf("Allocating %llu bytes\n", size);
#endif
 	return malloc(size);
}

void operator delete(void* pointer, size_t size) {
	Battery::MemoryUtils::totalBytesAllocated -= size;
#ifdef BATTERY_ENABLE_ALLOCATION_PRINTS
	printf("Freeing %llu bytes\n", size);
#endif
	return free(pointer);
}

#endif 

#endif
