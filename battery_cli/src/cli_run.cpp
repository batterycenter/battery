
#include "battery_cli.h"

Err cli_run() {
    log::info("battery run command");
    return { Result::SUCCESS, "" };
}
