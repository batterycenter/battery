
#include "battery/main.hpp"
#include "battery/log.hpp"
#include "battery/lockfile.hpp"
#include "battery/time.hpp"

int b::main([[maybe_unused]] const std::vector<b::string>& args) {

    b::log::info("Running Example Lockfile. To test it, start this example multiple times. Have fun!");

    b::log::debug("Waiting for 'lockfile.lock' to be locked");
    b::scoped_lockfile lock("lockfile.lock");

    b::log::debug("Locked 'lockfile.lock' successfully. We have exclusive access now!");
    for (int i = 0; i < 10; i++) {
        b::print("Performing critical operations in a folder...\n");
        b::sleep(1);
    }
    b::log::debug("Critical operations finished. Destructor unlocks the lockfile and allows another process to lock it.");

    b::print("\n");
    b::log::info("Example Lockfile finished. Please take a look at how it is implemented. Happy coding!");

    return 0;
}
