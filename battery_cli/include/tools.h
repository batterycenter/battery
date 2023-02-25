#pragma once

#include "pch.hpp"
#include "app.h"

std::optional<fs::path> find_project_root();

std::expected<fs::path, Err> project_root();

std::expected<toml::value, Err> parse_project_file(const fs::path& project_root);

std::expected<ProjectData, Err> extract_project_file_data(const fs::path& root, const toml::value& project_file);

std::expected<ProjectData, Err> fetch_project_data();
