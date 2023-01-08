
#include "battery_cli.h"

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
