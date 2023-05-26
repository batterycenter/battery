#pragma once

#include <optional>
#include "battery/core/messages.hpp"
#include "battery/core/fs.hpp"

namespace b {

    class filedialog {
    public:
        bool directory = false;
        bool confirm_overwrite = true;
        std::string default_extension;      // Format: "txt"
        b::fs::path initial_folder;
        std::string separator;      // WHAAATTT (TODO)
        std::vector<std::pair<std::string, std::string>> filters = { { "All files", "*.*" } };  // TODO: Allow this text to be localized

        filedialog() = default;

        b::fs::path sync_open();
        std::vector<b::fs::path> sync_open_multiple();
        b::fs::path sync_save();
    };

}