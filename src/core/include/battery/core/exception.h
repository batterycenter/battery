#pragma once

#include <exception>
#include <string>
#include <functional>

#include "battery/core/messages.h"
#include "battery/core/log.h"

namespace battery {

	class exception : public std::exception {
	public:
        exception(const std::string& msg = "") : msg(msg) {}
		const char* what() const noexcept override {
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

    template<typename T>
    void catch_common_exceptions(T func) {
        try {
            func();
        }
        catch (const battery::exception& e) {
            battery::log::core::critical("[battery::exception]: {}", e.what());
            battery::message_box_error(std::format("[battery::exception]: {}", e.what()));
        }
        catch (const std::exception& e) {
            battery::log::core::critical("[std::exception]: {}", e.what());
            battery::message_box_error(std::format("[std::exception]: {}", e.what()));
        }
        catch (...) {
            battery::log::core::critical("Unidentifiable exception caught, no further information");
            battery::message_box_error("Unidentifiable exception caught, no further information");
        }
    }

}
