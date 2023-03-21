
#include "console.h"
#include "messages.h"
#include "Project.h"

#include <conio.h>

Project::Project() {
    scripts["configure"] = "cmake -B {{ build_directory }} -S {{ source_directory }}";
    scripts["build"] = "cmake --build -B {{ build_directory }}";
    scripts["run"] = "echo Program running :)";
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

    battery::log::error(MESSAGES_TOML_NOT_FOUND, BATTERY_PROJECT_FILE_NAME);
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
            battery::log::warn(MESSAGES_CANNOT_READ_VERSION_FILE, projectRoot + b::toml::find<std::string>(toml, "version_file"));
            return b::unexpected(Error::VERSION_FILE_NOT_FOUND);
        }
        try {
            this->projectVersion = b::semver::from_string(version.value());
        }
        catch (const std::exception& e) {
            battery::log::warn(MESSAGES_INVALID_VERSION, version.value(), e.what());
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

    battery::log::info("Battery project file found at '{}'", projectRoot);
    battery::log::info("Project: {}", projectName);
    battery::log::info("Version: {}", projectVersion.to_string());

    return std::nullopt;
}

b::expected<std::nullopt_t, Error> Project::generateNewProject() {

    show_options({ "Option 1", "Option 2", "Option 3" });

//    while (b::time() < 10) {
//        //battery::log::warn("Info: {}", magic_enum::enum_name(b::console::get_control_key()));
//    }

    return std::nullopt;
}

b::expected<std::nullopt_t, Error> Project::runScript(const std::string& script) {
    return std::nullopt;
}
