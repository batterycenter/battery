
#include "battery/core/all.hpp"

#include "resources/message.txt.h"
#include "resources/banner.txt.h"

int b::main(const std::vector<std::string>& args) {

    b::log::info("BatteryEmbed example");

    b::print("\n{}\n\n", resources::banner_txt.str());
    b::print("{}\n", resources::message_txt.str());

    return 0;
}
