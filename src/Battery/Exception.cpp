
#include "Battery/pch.h"
#include "Battery/Core/Exception.h"

namespace Battery {

	Exception::Exception(const std::string& msg) {
		this->msg = msg;
	}

	const char* Exception::what() const noexcept {
		return msg.c_str();
	}




	LockfileUnavailableException::LockfileUnavailableException(const std::string& msg) : Battery::Exception(msg.c_str()) {
	}

}
