
#include "battery/battery.hpp"
#include "resources/another_folder/message_txt.hpp"
#include "resources/banner_txt.hpp"

int b::main(const std::vector<b::string>& args) {

    b::log::info("BatteryEmbed example");

    b::print("\n{}\n\n", resources::banner_txt.str());
    b::print("{}\n", resources::another_folder::message_txt.str());

    return 0;
}
