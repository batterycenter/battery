
#include "ProjectGenerator.h"
#include "console.h"

#include "resources/battery_cli/version.hpp"
#include "resources/battery_cli/templates/gitignore.hpp"
#include "resources/battery_cli/templates/CMakeLists_txt.hpp"
#include "resources/battery_cli/templates/CPM_cmake.hpp"
#include "resources/battery_cli/templates/battery_toml.hpp"
#include "resources/battery_cli/templates/version_txt.hpp"
#include "resources/battery_cli/templates/main_cpp.hpp"
#include "resources/battery_cli/templates/pch_hpp.hpp"

static int getSelectionInput(const b::string& question, const std::vector<b::string>& options) {
    b::print("{}\n", question);
    auto result = ask_user_options(options);
    b::print("\n");
    return result;
}

static b::string getSimpleInput(const b::string& question, const b::string& error_message) {
    b::print("{}\n", question);
    auto input = b::cin_getline();
    input = u8"Test";

    if (!input.empty()) {
        b::print("\n");
        return input;
    }

    b::print(b::colors::fg::red, "{}\n", error_message);
    return getSimpleInput(question, error_message);
}

static auto getConvertableInput(const b::string& question, fmt::format_string<b::string> error_message, auto&& converter) {
    b::print("{}\n", question);
    auto input = b::cin_getline();

    try {
        auto result = converter(input);
        if (!input.empty()) {
            b::print("\n");
        }
        return result;
    }
    catch (const std::invalid_argument&) {
        b::print(b::colors::fg::red, error_message, std::forward<decltype(input)>(input));
        b::print("\n");
        return getConvertableInput(question, error_message, converter);
    }
}

b::expected<std::nullopt_t, Error> ProjectGenerator::run(const b::string& new_dir) {

    // Type of project
    library = 1 == getSelectionInput(
        "What type of project do you want to generate?", {
            "Application (One or more executables)",
            "Library (For use in other applications)"
        });

    if (library) {
        b::print(b::colors::fg::red, "Library generation is not yet implemented!\n");
        return b::unexpected(Error::NOT_YET_IMPLEMENTED);
    }

    // Project name
    project_name = getSimpleInput("What is the name of your project?", "Please enter a project name!");

    // Project version
    project_version = getConvertableInput(
        "What is the version of your project? [1.0.0]",
        "'{}' is not a valid version for semantic versioning! (example: 1.0.0 or 3.2.7-alpha.2)",
        [](const auto& input) {
            return b::semver::from_string(input.empty() ? "1.0.0" : input);
        }
    );

    // Project generation directory
    auto path = b::fs::path(new_dir.empty() ? project_name : new_dir);
    project_dir = path.is_absolute() ? path : b::fs::current_path() + path;

    b::print("Project generation summary:\n");
    b::print("name = {}\n", project_name);
    b::print("version = {}\n", project_version.to_string());
    b::print("project_dir = {}\n", project_dir);
    b::print("\n");

    auto confirm = getConvertableInput(
            "Do you want to generate the project? [Y/n]",
            "'{}' is not a valid answer! (Y/n)",
            [](const auto& input) {
                if (input.empty() || input == "Y" || input == "y") {
                    return true;
                }
                if (input == "N" || input == "n") {
                    return false;
                }
                throw std::invalid_argument("");
            }
    );

    if (confirm) {
        b::print("Generating project...\n");
        auto result = generate();
        if (!result) return b::unexpected(result.error());
        b::print("Project files generated in {}\n", project_dir);
    }
    else {
        b::print(b::colors::fg::yellow, "Project generation cancelled. No files were generated.\n");
    }

    return std::nullopt;
}







b::expected<b::string, Error> ProjectGenerator::generateSourceFile(const b::string& name, b::string content) {
    nlohmann::json properties;
    properties["project_name"] = project_name;
    properties["project_version"] = project_version.to_string();

    b::string old;
    try {
        while (old != content) {
            old = content;
            content = b::inja::render(content, properties);
        }
    }
    catch (const std::exception& e) {
        b::log::warn("Internal Error: The template '{}' failed to parse. Reason:", name);
        b::print(b::colors::fg::blue, "{}\n", e.what());
        return b::unexpected(Error::INTERNAL_ERROR);
    }
    return content;
}

#define GENERATE_FILE(var, file, resource) { \
        auto result = generateSourceFile(file, resource); \
        if (result) var = result.value(); \
        else return b::unexpected(result.error()); \
    }

b::expected<std::nullopt_t, Error> ProjectGenerator::generate() {     // This function loads every file as a string, and generates them from templates.
    GENERATE_FILE(this->gitignore, ".gitignore", Resources::battery_cli::templates::gitignore);
    GENERATE_FILE(this->cmakelists, "CMakeLists.txt", Resources::battery_cli::templates::CMakeLists_txt);
    GENERATE_FILE(this->cmake_cpm_cmake, "cmake/CPM.cmake", Resources::battery_cli::templates::CPM_cmake);
    GENERATE_FILE(this->battery_toml, "battery.toml", Resources::battery_cli::templates::battery_toml);
    GENERATE_FILE(this->version_txt, "version.txt", Resources::battery_cli::templates::version_txt);
    GENERATE_FILE(this->main_cpp, "src/main.cpp", Resources::battery_cli::templates::main_cpp);
    GENERATE_FILE(this->pch_hpp, "include/pch.hpp", Resources::battery_cli::templates::pch_hpp);
    return writeToDisk();
}




b::expected<std::nullopt_t, Error> ProjectGenerator::writeSourceFile(const b::fs::path& filename, const b::string& data) {
    b::fs::ofstream file(project_dir + filename);
    if (file.fail()) {
        b::log::error("Failed to open file '{}' for writing. Project files are incomplete.", filename);
        return b::unexpected(Error::FILE_WRITE_ERROR);
    }
    file << data;
    return std::nullopt;
}

#define WRITE_FILE(var, file) { \
        auto result = writeSourceFile(file, var); \
        if (!result) return b::unexpected(result.error()); \
    }

b::expected<std::nullopt_t, Error> ProjectGenerator::writeToDisk() {
    WRITE_FILE(this->gitignore, ".gitignore");
    WRITE_FILE(this->cmakelists, "CMakeLists.txt");
    WRITE_FILE(this->cmake_cpm_cmake, "cmake/CPM.cmake");
    WRITE_FILE(this->battery_toml, "battery.toml");
    WRITE_FILE(this->version_txt, "version.txt");
    WRITE_FILE(this->main_cpp, "src/main.cpp");
    WRITE_FILE(this->pch_hpp, "include/pch.hpp");
    return std::nullopt;
}
