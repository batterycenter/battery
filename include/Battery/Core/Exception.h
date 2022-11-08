#pragma once

#include "Battery/pch.hpp"

#define BATTERY_EXCEPTION UI_EXCEPTION

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

	class NoSuchFileOrDirectoryException : public Battery::Exception {
	public:
		NoSuchFileOrDirectoryException(const std::string& msg = "")
		 : Battery::Exception(("Battery::NoSuchFileOrDirectoryException: " + msg).c_str()) {}
	};

	class LockfileUnavailableException : public Battery::Exception {
	public:
		LockfileUnavailableException(const std::string& msg = "")
		 : Battery::Exception(("Battery::LockfileUnavailableException: " + msg).c_str()) {}
	};

	class NotImplementedException : public Battery::Exception {
	public:
		NotImplementedException(const std::string& msg = "")
		 : Battery::Exception(("Battery::NotImplementedException: " + msg).c_str()) {}
	};

}
