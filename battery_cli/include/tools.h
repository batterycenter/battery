#pragma once

#include "pch.hpp"
#include "app.h"

std::optional<b::fs::path> find_project_root();

std::pair<b::fs::path, Err> project_root();

std::pair<toml::value, Err> parse_project_file(const b::fs::path& project_root);

std::pair<ProjectData, Err> extract_project_file_data(const b::fs::path& root, const toml::value& project_file);

std::pair<ProjectData, Err> fetch_project_data();
