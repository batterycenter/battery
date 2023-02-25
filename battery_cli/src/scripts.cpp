
#include "pch.hpp"
#include "tools.h"
#include "scripts.h"
#include "app.h"

Err load_default_scripts(ProjectData& project) {
    project.scripts["configure"] = "cmake ..";
    project.scripts["build"] = "cmake --build .";
    project.scripts["run"] = "cmake --build .";
    return { Result::SUCCESS, "" };
}

Err run_script(const ProjectData& project, const std::string& script) {
    cmake_build(project);
    return { Result::SUCCESS, "" };
}
