
#include "ProjectGenerator.h"
#include "console.h"

void ProjectGenerator::writeToDisk() {

}

b::expected<std::nullopt_t, Error> ProjectGenerator::run() {
    std::cout <<"What type of project do you want to generate?" << std::endl << std::endl;
    bool library = ask_user_options({ "Application (One or more executables)", "Library (For use in other applications)" }) == 1;
    std::cout << std::endl;
    b::log::info("Library: {}", library);
    return std::nullopt;
}
