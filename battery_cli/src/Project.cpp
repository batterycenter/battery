
#include "messages.h"
#include "Project.h"
#include "ProjectGenerator.h"

bool Project::isProjectConfigured() {
    try {
        return projectCache["configured"];
    }
    catch (...) {
        return false;
    }
}

b::expected<std::nullopt_t, Error> Project::init(const std::string& cmake_flags, const std::string& root, const std::string& args) {

    scripts["pre_configure"] = "exit 0";
    scripts["configure"] = "b run pre_configure && cmake -B {{build_directory}} -S {{project_root}} -DCMAKE_BUILD_TYPE={{config}} {{cmake_flags}} && b run post_configure";
    scripts["post_configure"] = "exit 0";

    scripts["pre_build"] = "exit 0";
    scripts["build"] = "b configure --cache && b run pre_build && cmake --build {{build_directory}} --config={{config}} {{cmake_flags}} && b run post_build";
    scripts["post_build"] = "exit 0";

    scripts["pre_start"] = "exit 0";
    scripts["start"] = "b build && b run pre_start && b execute {{project_root}}/{{executable}} {% if length(args) > 0 %} --args {{args}} {% endif %} && b run post_start";
    scripts["post_start"] = "exit 0";

    data["build_directory"] = "build";
    data["binary_directory"] = "{{build_directory}}/bin/{{config}}";
    data["executable_name"] = "{{project_name}}";
    data["executable"] = "{{binary_directory}}/{{executable_name}}";
    data["config"] = "debug";
    data["config_package"] = "release";

    auto result = fetchProjectData(root);
    if (!result) {
        return b::unexpected(result.error());
    }

    data["project_name"] = projectName;
    data["project_root"] = projectRoot.to_string();
    data["project_version"] = projectVersion.to_string();
    data["cmake_flags"] = cmake_flags;
    data["args"] = args;

    projectCache = b::cachefile(projectRoot + data["build_directory"] + "battery.cache");
    return std::nullopt;
}

b::expected<std::nullopt_t, Error> Project::findProjectRoot(const std::string& root) {
    b::fs::path path = b::fs::current_path();       // TODO: This type is still wrong (b::fs todo)

    if (!root.empty()) {
        path = root;
    }

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

    b::log::warn(MESSAGES_TOML_NOT_FOUND, BATTERY_PROJECT_FILE_NAME);
    return b::unexpected(Error::PROJECT_FILE_NOT_FOUND);
}

b::expected<std::nullopt_t, Error> Project::fetchProjectData(const std::string& root) {

    auto success = findProjectRoot(root);
    if (!success) { return b::unexpected(success.error()); }

    try {
        // Read toml file and get project name
        auto toml = b::toml::parse(projectRoot + BATTERY_PROJECT_FILE_NAME);
        this->projectName = b::toml::find<std::string>(toml, "project_name");

        // Overrides from the toml file -> All json values are directly overridable
        for (const auto& entry : data.items()) {
            if (toml.contains(entry.key())) {
                data[entry.key()] = b::toml::find<std::string>(toml, entry.key());
            }
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

    return std::nullopt;
}

void Project::printScriptLabel(const std::string& script) {
    b::print(b::print_color::GREEN, ">> {} v{} {}", projectName, projectVersion.to_string(), script);
}

b::expected<std::nullopt_t, Error> Project::runScript(std::string script) {

    if (script.empty()) {   // 'b run' is equivalent to 'b run start' or 'b start'
        script = "start";
    }

    if (!scripts.contains(script)) {
        b::log::warn("'{}' is neither a default script, nor is it defined in {}", script, BATTERY_PROJECT_FILE_NAME);
        return b::unexpected(Error::SCRIPT_NOT_FOUND);
    }

    if (scripts[script] == "exit 0") {   // Do not run the script if it does not do anything (default)
        return std::nullopt;
    }

    if (script == "configure") {
        projectCache["configured"] = false;
    }

    b::print("");
    printScriptLabel(script);

    std::string old;
    std::string command = scripts[script];
    try {
        while (old != command) {
            old = command;
            command = b::inja::render(command, data);
        }
    }
    catch (const std::exception& e) {
        b::log::warn("The command '{}' failed to parse. Reason:", scripts[script]);
        b::print(b::print_color::BLUE, "{}", e.what());
        return b::unexpected(Error::SCRIPT_PARSE_ERROR);
    }

    b::print(">> {}", command);

    b::process process;
    process.options.passthrough_to_parent = true;
    process.options.working_directory = projectRoot;
    process.options.execute_as_shell_command = true;
    process.options.arguments = { command };

    terminateCallback = [&process]() { process.terminate(); };
    process.execute_sync();
    terminateCallback = {};

    if (process.exit_code != 0) {
        b::log::warn("Script failed with error code {}", process.exit_code);   // We no longer print the 'error message' since it's always the same
        return b::unexpected(Error::SCRIPT_FAILED);                           // We call bash or cmd underneath which practically never fail themselves
    }

    if (script == "configure") {    // If the script was 'configure', we write to the cache that the project is configured
        projectCache["configured"] = true;
    }

    return std::nullopt;
}
