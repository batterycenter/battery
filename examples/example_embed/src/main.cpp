
#include "Battery/Battery.h"
using namespace Battery;

#include "resources/message_txt.h"
#include "resources/banner_txt.h"

int Battery::Main(const Args_t& args) {

    Log::Info("Battery-Embed example");
    std::cout << std::endl;

    std::cout << resources::banner_txt << std::endl;
    std::cout << std::endl;

    std::cout << resources::message_txt << std::endl;

    return 0;
}
