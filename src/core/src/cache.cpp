
#include "battery/core/cache.hpp"
#include "battery/core/fs.hpp"

namespace b {

    cachefile::cachefile() {

    }

    cachefile::cachefile(const b::fs::path& filename) : filename(filename) {
        if (!b::fs::is_regular_file(filename)) {
            b::fs::ofstream file(filename);
            if (file.fail()) {
                throw std::runtime_error("b::cachefile: Failed to open file '" + b::u8_as_str(filename.u8string()) + "' for writing: " + strerror(errno));
            }
            file << nlohmann::json::parse("{}").dump(4);
            file.close();
        }
    }

}