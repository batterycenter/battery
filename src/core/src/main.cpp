
#include "battery/core/environment.h"
#include "battery/core/main.h"
#include "battery/core/log.h"
#include "battery/core/platform.h"
#include "battery/core/OsString.h"
#include "battery/core/exception.h"

namespace battery {
	
    const char** args_to_cstr(const Args_t& _args) {
        static Args_t args_raw = _args;     // Do this so the data will stay in memory forever (needed by c_str())
        static std::vector<const char*> arguments;
        
		for (auto& arg : args_raw) {
			arguments.push_back(arg.c_str());
		}
        return arguments.data();
    }
		
}




int run_main(const battery::Args_t& args) {
    try {
        // And now call the actual entry point, defined by the user
//        return battery::main(args);
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
    return -1;
}
