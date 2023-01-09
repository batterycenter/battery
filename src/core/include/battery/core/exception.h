#pragma once

#define BATTERY_EXCEPTION UI_EXCEPTION

#include <exception>
#include <string>

namespace battery {

	class exception : public std::exception {
	public:
        exception(const std::string& msg = "") : msg(msg) {}
		const char* what() const noexcept {
			return msg.c_str();
		}
	
	private:
		std::string msg;
	};

	class no_such_file_or_directory_exception : public battery::exception {
	public:
        no_such_file_or_directory_exception(const std::string& msg = "")
		 : battery::exception(("battery::no_such_file_or_directory_exception: " + msg).c_str()) {}
	};

	class lockfile_unavailable_exception : public battery::exception {
	public:
        lockfile_unavailable_exception(const std::string& msg = "")
		 : battery::exception(("battery::lockfile_unavailable_exception: " + msg).c_str()) {}
	};

    class deadlock_exception : public battery::exception {
    public:
        deadlock_exception(const std::string& msg = "")
                : battery::exception(("battery::deadlock_exception: " + msg).c_str()) {}
    };

	class not_implemented_exception : public battery::exception {
	public:
        not_implemented_exception(const std::string& msg = "")
		 : battery::exception(("battery::not_implemented_exception: " + msg).c_str()) {}
	};

}
