#pragma once

#include "Battery/common.h"
#include "Battery/Core/log.hpp"
#include "Battery/Core/Config.h"

namespace Battery {

	/// <summary>
	/// Convert a Unix timestamp like '2021-01-02T10:34:52Z' to an epoch number, which is 1609580092 in this case.
	/// Returns 0 on failure
	/// </summary>
	/// <param name="timestamp">- Unix timestamp in string format</param>
	/// <returns>std::time_t - The epoch time in seconds or 0 on failure</returns>
	std::time_t ConvertTimestamp(const std::string& timestamp);

	/// <summary>
	/// Get the time since the Application has been initalized, if not initialized, returns 0.
	/// </summary>
	/// <returns>Time since engine is running</returns>
	sf::Time GetRuntime();

	/// <summary>
	/// Reset the runtime counter to 0. WARNING: Only to be used internally!!
	/// </summary>
	void ClearRuntime();
}
