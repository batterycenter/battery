#pragma once

#include "Battery/pch.h"

namespace Battery {

	class Exception : public std::exception {
	public:
		Exception(const std::string& msg = "") : msg(msg) {}
		const char* what() const noexcept {
			return msg.c_str();
		}
	
	private:
		std::string msg;
	};

	class LockfileUnavailableException : public Battery::Exception {
	public:
		LockfileUnavailableException(const std::string& msg = "")
		 : Battery::Exception(msg.c_str()) {}
	};

	class NotImplementedException : public Battery::Exception {
	public:
		NotImplementedException(const std::string& msg = "")
		 : Battery::Exception(msg.c_str()) {}
	};

}
