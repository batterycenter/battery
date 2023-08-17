
// #include "battery/core/all.hpp"

// void test() {
//     b::log::warn("No handler");
//     b::sleep(2);
//     b::push_ctrl_c_handler([] { b::log::info("Ctrl+C Handler 1"); });
//     b::log::warn("Handler 1");
//     b::sleep(2);
//     b::push_ctrl_c_handler([] { b::log::info("Ctrl+C Handler 2"); });
//     b::log::warn("Handler 2");
//     b::sleep(2);
//     b::push_ctrl_c_handler([] { b::log::info("Ctrl+C Handler 3"); });
//     b::log::warn("Handler 3");
//     b::sleep(2);

//     b::pop_ctrl_c_handler();
//     b::log::warn("Handler 3 deleted");
//     b::sleep(2);

//     b::pop_ctrl_c_handler();
//     b::log::warn("Handler 2 deleted");
//     b::sleep(2);

//     b::pop_ctrl_c_handler();
//     b::log::warn("Handler 1 deleted");
//     b::sleep(2);

//     while(true) {
//         b::sleep(0.1);
//         b::log::info("loop");
//         b::print("loop\n");
//     }
// };

#include "battery/core.hpp"

int b::main([[maybe_unused]] const std::vector<b::string>& args) {



    return 0;
}
