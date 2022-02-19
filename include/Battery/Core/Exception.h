#pragma once

#include "Battery/pch.h"

namespace Battery {

	/// <summary>
	/// Custom exception class used in the entire engine
	/// </summary>
	class Exception : public std::exception {
	public:
		Exception(const std::string& msg);
	};

	/// <summary>
	/// Lockfile cannot be retrieved by the Battery::Lockfile class: It's already locked
	/// </summary>
	class LockfileUnavailableException : public Battery::Exception {
	public:
		LockfileUnavailableException(const std::string& msg);
	};

}
