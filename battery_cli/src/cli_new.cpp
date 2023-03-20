
#include "tools.h"

Err cli_new(const ProjectData& project) {
    battery::log::info("battery new command");
    return { Result::SUCCESS, "" };
}
