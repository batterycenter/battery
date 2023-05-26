
#include <battery/core/main.hpp>
#include "App.hpp"

int b::main(const std::vector<b::string>& args) {
    App app;
    app.run(u8"Battery_SimpleGraphicsExample", args);
    return 0;
}
