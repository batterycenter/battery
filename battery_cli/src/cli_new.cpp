
#include "battery_cli.h"

Err cli_new() {
    log::info("battery new command");
    return { Result::SUCCESS, "" };
}
