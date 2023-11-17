
#include "battery/main.hpp"
#include "App.hpp"

int b::main(const std::vector<b::string>& args) {
    App app;
    return app.run("Battery_SimpleGraphicsExample", args);
}
