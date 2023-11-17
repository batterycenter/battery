
#include "battery/battery.hpp"

int b::main(const std::vector<b::string>& args) {

    b::log::info("Running CustomMain example!");
    b::log::info("This feature allows better integration into existing frameworks, "
                 "but is highly discouraged whenever possible as there are no compatibility guarantees across platforms.");
    b::log::debug("You are highly encouraged to use b::main() and use b::args_to_argv(...) if you need C-style arguments for legacy APIs.");
    b::log::debug("b::args_to_argv(...) is guaranteed to be UTF-8 encoded on any platform.");

    auto argc = args.size();
    auto argv = b::args_to_argv(args);

    (void)argc;
    (void)argv;

    return 0;
}

int main(int argc, const char** argv) {
    b::print("doing very important stuff before main...\n");
    return b::invoke_main(argc, argv);
}
