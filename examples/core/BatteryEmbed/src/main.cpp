
#include "battery/core/all.hpp"

#include "resources/another_folder_message_txt.h"
#include "resources/banner_txt.h"

int b::main(const std::vector<std::u8string>& args) {

    b::log::info("BatteryEmbed example");

    b::print("\n{}\n\n", resources::banner_txt.str());
    b::print("{}\n", resources::another_folder_message_txt.str());

    return 0;
}
