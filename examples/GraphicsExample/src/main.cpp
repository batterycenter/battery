
#include <battery/core/main.hpp>
#include "App.hpp"

int b::main(const std::vector<std::u8string>& args) {
    App app;
    app.run(u8"Battery_GraphicsExample", args);
    return 0;
}
