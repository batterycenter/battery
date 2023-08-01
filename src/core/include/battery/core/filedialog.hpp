#pragma once

#include "battery/core.hpp"

namespace b {

    class filedialog {
    public:
        bool directory = false;
        bool confirm_overwrite = true;
        b::fs::path default_extension;      // Format: "txt"
        b::fs::path initial_folder;
        b::string separator;      // WHAAATTT (TODO)
        std::vector<std::pair<b::string, b::string>> filters = { { "All files", "*.*" } };  // TODO: Allow this text to be localized

        filedialog() = default;

        b::fs::path sync_open();
        std::vector<b::fs::path> sync_open_multiple();
        b::fs::path sync_save();
    };

}