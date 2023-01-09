
#include "tools.h"

Err cli_configure() {

    auto project_data_opt = fetch_project_data();
    if (!project_data_opt) return project_data_opt.error();
    ProjectData project = project_data_opt.value();

    log::info("Battery project file found at {}:", project.project_root.to_string());
    log::info("Project: {}", project.project_name);
    log::info("Version: {}.{}.{}", project.project_version.major, project.project_version.minor, project.project_version.patch);

    battery::process process;
    process.execute();

    return { Result::SUCCESS, "" };
}
