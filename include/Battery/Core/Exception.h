#pragma once

#include "Battery/pch.h"

namespace Battery {

	class Exception : public std::exception {
	public:
		Exception(const std::string& msg);
		const char* what() const noexcept;
	
	private:
		std::string msg;
	};

	class LockfileUnavailableException : public Battery::Exception {
	public:
		LockfileUnavailableException(const std::string& msg);
	};

}
