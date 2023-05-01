
#include <battery/core/main.hpp>
#include "App.hpp"

int b::main(const std::vector<std::string>& args) {
    App app;
    app.run("Battery_GraphicsExample", args);
    return 0;
}
