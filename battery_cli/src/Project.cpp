
#include "console.h"
#include "messages.h"
#include "Project.h"

#include <conio.h>

Project::Project() {
    scripts["configure"] = "cmake -B {{build_directory}} -S {{source_directory}} {{cmake_flags}}";
    scripts["build"] = "cmake --build -B {{build_directory}} {{cmake_flags}}";
    scripts["start"] = "echo Program running :)";
}

b::expected<std::nullopt_t, Error> Project::findProjectRoot() {
    b::fs::path path = b::fs::current_path();       // TODO: This type is still wrong (b::fs todo)

    if (b::fs::is_regular_file(path + BATTERY_PROJECT_FILE_NAME)) {
        this->projectRoot = path;
        return std::nullopt;
    }

    while (path.has_parent_path()) {
        auto old = path;
        path = path.parent_path();  // We need to check it this way. If the path is "C:/", has_parent_path() still is true
        if (path == old) {
            break;
        }
        if (b::fs::is_regular_file(path + BATTERY_PROJECT_FILE_NAME)) {
            this->projectRoot = path;
            return std::nullopt;
        }
    }

    b::log::error(MESSAGES_TOML_NOT_FOUND, BATTERY_PROJECT_FILE_NAME);
    return b::unexpected(Error::PROJECT_FILE_NOT_FOUND);
}

b::expected<std::nullopt_t, Error> Project::fetchProjectData() {

    auto success = findProjectRoot();
    if (!success) { return b::unexpected(success.error()); }

    try {
        // Read toml file and get project name
        auto toml = b::toml::parse(projectRoot + BATTERY_PROJECT_FILE_NAME);
        this->projectName = b::toml::find<std::string>(toml, "project_name");

        // Read a few directories
        if (toml.contains("build_directory")) {
            this->buildDirectory = b::toml::find<std::string>(toml, "build_directory");
        }
        if (toml.contains("source_directory")) {
            this->sourceDirectory = b::toml::find<std::string>(toml, "source_directory");
        }

        // Parse the version
        auto version = b::fs::ifstream(projectRoot + b::toml::find<std::string>(toml, "version_file")).read_string();
        if (!version) {
            b::log::warn(MESSAGES_CANNOT_READ_VERSION_FILE, projectRoot + b::toml::find<std::string>(toml, "version_file"));
            return b::unexpected(Error::VERSION_FILE_NOT_FOUND);
        }
        try {
            this->projectVersion = b::semver::from_string(version.value());
        }
        catch (const std::exception& e) {
            b::log::warn(MESSAGES_INVALID_VERSION, version.value(), e.what());
            return b::unexpected(Error::INVALID_VERSION);
        }

        // Parse the supplied scripts
        if (toml.contains("scripts")) {
             for (auto& [label, command] : toml::find(toml, "scripts").as_table()) {
                 scripts[label] = command.as_string().str;
             }
        }
    }
    catch (const std::exception& e) {
        std::cout << rang::fgB::blue << e.what() << rang::fg::reset << std::endl;
        return b::unexpected(Error::TOML_PARSE_ERROR);
    }

    b::print(b::print_color::GREEN, "Project {} v{}", projectName, projectVersion.to_string());

    return std::nullopt;
}

b::expected<std::nullopt_t, Error> Project::generateNewProject() {

    std::cout <<"What type of project do you want to generate?" << std::endl << std::endl;
    bool library = ask_user_options({ "Application (One or more executables)", "Library (For use in other applications)" }) == 1;
    std::cout << std::endl;
    b::log::info("Library: {}", library);

//    while (b::time() < 10) {
//        //battery::log::warn("Info: {}", magic_enum::enum_name(b::console::get_control_key()));
//    }

    return std::nullopt;
}

b::expected<std::nullopt_t, Error> Project::runScript(std::string script) {

    if (script.empty()) {   // 'b run' is equivalent to 'b run start' or 'b start'
        script = "start";
    }

    nlohmann::json data;
    data["project_name"] = projectName;
    data["project_version"] = projectVersion.to_string();
    data["build_directory"] = "\"" + (projectRoot + buildDirectory).to_string() + "\"";
    data["source_directory"] = "\"" + (projectRoot + sourceDirectory).to_string() + "\"";
    data["cmake_flags"] = cmakeFlags;

    if (!scripts.contains(script)) {
        b::log::warn("'{}' is neither a default script, nor is it defined in {}", script, BATTERY_PROJECT_FILE_NAME);
        return b::unexpected(Error::SCRIPT_NOT_FOUND);
    }

    b::print("Running script '{}'", script);
    std::string command;
    try {
        command = b::inja::render(scripts[script], data);
    }
    catch (const std::exception& e) {
        b::log::warn("The command '{}' failed to parse. Reason:", scripts[script]);
        std::cout << rang::fgB::blue << e.what() << rang::fg::reset << std::endl;
        return b::unexpected(Error::SCRIPT_PARSE_ERROR);
    }

    b::print("{}", command);
    b::log::error("{}", command);
    return std::nullopt;
}
