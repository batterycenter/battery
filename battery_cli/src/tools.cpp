
#include "app.h"
#include "scripts.h"

std::optional<fs::path> find_project_root() {
    auto path = std::filesystem::current_path();

    do {
        auto p = path;
        p.append(BATTERY_PROJECT_FILE_NAME);
        if (fs::status(p).type() == std::filesystem::file_type::regular) {    // Check if it's a regular file
            return path;
        }

        auto previous = path;
        path = path.parent_path();
        if (path == previous) {     // We seem to have reached the topmost directory, parent doesn't change anymore
            break;
        }
    }
    while (!path.empty());

    return {};
}

#define TRY_TOML(expr) \
    try { \
        expr; \
    } \
    catch (const std::exception& e) { \
        std::cout << rang::fgB::blue << e.what() << rang::fg::reset << std::endl; \
        return std::unexpected(Err{ Result::PROJECT_FILE_INVALID, "The project file " + BATTERY_PROJECT_FILE_NAME + " is invalid" }); \
    }

std::expected<fs::path, Err> project_root() {
    auto path_opt = find_project_root();
    if (!path_opt.has_value()) {
        return std::unexpected(Err{ Result::PROJECT_FILE_NOT_FOUND,
                                    BATTERY_PROJECT_FILE_NAME +
                                    " not found in any of the parent directories. Make sure you are on the correct path, "
                                    "otherwise use 'battery new' to generate a new project." });
    }
    return path_opt.value();
}

std::expected<toml::value, Err> parse_project_file(const fs::path& project_root) {
    fs::path project_file_path = fs::path(project_root).append(BATTERY_PROJECT_FILE_NAME);

    TRY_TOML(
            return toml::parse(project_file_path);
    );
}

std::expected<ProjectData, Err> extract_project_file_data(const fs::path& root, const toml::value& project_file) {
    ProjectData data;

    TRY_TOML(data.project_name = toml::find<std::string>(project_file, "project_name"));
    TRY_TOML(
            auto& str = toml::find<std::string>(project_file, "project_version");
            data.project_version = semver::from_string(str)
    );
    TRY_TOML(data.cmake_path = toml::find<std::string>(project_file, "cmake_path"));

    load_default_scripts(data);

    // Parse scripts table
    TRY_TOML({
        if (project_file.contains("scripts")) {
            auto& table = toml::find(project_file, "scripts");
            for (auto& [label, command] : toml::find(project_file, "scripts").as_table()) {
                data.scripts[label] = command.as_string().str;
            }
        }
    });

    data.project_root = root;

    return data;
}

std::expected<ProjectData, Err> fetch_project_data() {

    // Find the root of the project
    auto project_root_opt = project_root();
    if (!project_root_opt) return std::unexpected(project_root_opt.error());
    fs::path project_root = project_root_opt.value();

    auto x = [&]() {
        if (project_root_opt)
            return project_root_opt.value();
    };

    // Now we know where the project lives, parse project file
    auto project_file_opt = parse_project_file(project_root);
    if (!project_file_opt) return std::unexpected(project_file_opt.error());
    toml::value project_file = project_file_opt.value();

    // Now interpret the project data
    return extract_project_file_data(project_root, project_file);

}
