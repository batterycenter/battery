
#include "battery_cli.h"

#define TRY_TOML(expr) \
    try { \
        expr; \
    } \
    catch (const std::exception& e) { \
        std::cout << rang::fgB::blue << e.what() << rang::fg::reset << std::endl; \
        return std::unexpected(Err{ Result::PROJECT_FILE_INVALID, "The project file " + BATTERY_PROJECT_FILE_NAME + " is invalid" }); \
    }

std::expected<FS::Path, Err> project_root() {
    auto path_opt = find_project_root();
    if (!path_opt.has_value()) {
        return std::unexpected(Err{ Result::PROJECT_FILE_NOT_FOUND, 
            BATTERY_PROJECT_FILE_NAME + 
            " not found in any of the parent directories. Make sure you are on the correct path, "
            "otherwise use 'battery new' to generate a new project." });
    }
    return path_opt.value();
}

std::expected<toml::value, Err> parse_project_file(const FS::Path& project_root) {
    Battery::FS::Path project_file_path = Battery::FS::Path(project_root).append(BATTERY_PROJECT_FILE_NAME);
	
    TRY_TOML(
        return toml::parse(project_file_path);
    );
}

struct ProjectData {
    std::string project_name;
    semver::version project_version;
};

std::expected<ProjectData, Err> extract_project_file_data(const toml::value& project_file) {
    ProjectData data;
	
    TRY_TOML(data.project_name = toml::find<std::string>(project_file, "project_name"));
    TRY_TOML(
        auto& str = toml::find<std::string>(project_file, "project_version");
        data.project_version = semver::from_string(str)
    );

    return data;
}

Err cli_configure() {

    // Find the root of the project
	auto project_root_opt = project_root();
	if (!project_root_opt) return project_root_opt.error();
    Battery::FS::Path project_root = project_root_opt.value();
	
    // Now we know where the project lives, parse project file
    auto project_file_opt = parse_project_file(project_root);
    if (!project_file_opt) return project_file_opt.error();
	toml::value project_file = project_file_opt.value();

	// Now interpret the project data
	auto project_data_opt = extract_project_file_data(project_file);
	if (!project_data_opt) return project_data_opt.error();
	ProjectData project_data = project_data_opt.value();

    Log::Info("Battery project file found at {}:", project_root.to_string());
    std::cout << project_file << std::endl;

    return { Result::SUCCESS, "" };
}
