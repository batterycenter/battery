
#include "battery/core/cache.hpp"
#include "battery/core/fs.hpp"

namespace b {

//    cachefile::cachefile() {
//
//    }
//
//    cachefile::cachefile(const b::fs::path& filename) : filename(filename) {
//        if (!b::fs::is_regular_file(filename)) {
//            b::fs::ofstream file(filename);
//            if (file.fail()) {
//                throw std::runtime_error(b::format("b::cachefile: Failed to open file '{}' for writing: ", filename, strerror(errno)));
//            }
//            file << nlohmann::json::parse("{}").dump(4);
//            file.close();
//        }
//    }

} // namespace b