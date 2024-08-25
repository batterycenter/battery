
#include "battery/cache.hpp"
#include "battery/fs.hpp"
#include "battery/folders.hpp"

namespace b {

static void init(const b::fs::path& filename)
{
    if (!b::fs::exists(filename)) {
        b::fs::write(filename, nlohmann::json::parse("{}").dump(4));
    }
}

static b::fs::path defaultFilename() { b::Folders::AppCacheDir() / "app.cache"; }

cachefile::cachefile()
    : filename(defaultFilename()) { init(filename) }

    cachefile::cachefile(const b::fs::path& filename)
    : filename(filename)
{
    init(filename)
}

} // namespace b