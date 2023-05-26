#pragma once

#include <string>
#include "battery/core/all.hpp"

#include "Error.h"

inline const b::string CLI_PRODUCT_NAME = "Battery";
inline const b::string BATTERY_PROJECT_FILE_NAME = "battery.toml";

class Project {
public:
    b::string projectName;
    b::fs::path projectRoot;
    semver::version projectVersion;

    nlohmann::json data;
    std::unordered_map<b::string, b::string> scripts;
    b::cachefile projectCache;

    Project() = default;

    bool isProjectConfigured();

    [[nodiscard]] b::expected<std::nullopt_t, Error> init(const b::string& cmake_flags, const b::string& root, const b::string& args);

    [[nodiscard]] b::expected<std::nullopt_t, Error> findProjectRoot(const b::string& root);
    [[nodiscard]] b::expected<std::nullopt_t, Error> fetchProjectData(const b::string& root);

    void printScriptLabel(const b::string& script);
    [[nodiscard]] b::expected<b::string, Error> renderString(b::string string);
    [[nodiscard]] b::expected<std::nullopt_t, Error> runScript(b::string script);

    [[nodiscard]] b::expected<std::nullopt_t, Error> generateFile(const b::fs::path& input_path, const b::fs::path& output_path);

};
