#pragma once

#include "pch.hpp"

Err load_default_scripts(ProjectData& project);

Err run_script(const ProjectData& project, const std::string& script);
