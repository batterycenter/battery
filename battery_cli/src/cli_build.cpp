
#include "battery_cli.h"

Err cli_build() {
    log::info("battery build command");
    return { Result::SUCCESS, "" };
}
