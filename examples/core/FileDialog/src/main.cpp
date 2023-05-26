
#include "battery/core/all.hpp"
#include "battery/filedialog/filedialog.hpp"

enum class Options {
    OPEN_FILE,
    OPEN_FILE_MULTIPLE,
    OPEN_FOLDER,
    OPEN_FOLDER_MULTIPLE,
    SAVE_FILE,
    QUIT
};

enum Options showOptions() {
    b::print("Options:\n");
    b::print(" - 'open-file': Open File-choosing UI\n");
    b::print(" - 'open-file-multi': Open File-choosing UI (multiple files)\n");
    b::print(" - 'open-folder': Open Folder-choosing UI\n");
    b::print(" - 'open-folder-multi': Open Folder-choosing UI (multiple folders)\n");
    b::print(" - 'save-file': Open File-choosing UI\n");
    b::print(" - 'quit': Quit the example\n");
    b::print("> ");

    auto result = b::cin_getline();
    if (result == u8"open-file") {
        return Options::OPEN_FILE;
    } else if (result == u8"open-file-multi") {
        return Options::OPEN_FILE_MULTIPLE;
    } else if (result == u8"open-folder") {
        return Options::OPEN_FOLDER;
    } else if (result == u8"open-folder-multi") {
        return Options::OPEN_FOLDER_MULTIPLE;
    } else if (result == u8"save-file") {
        return Options::SAVE_FILE;
    } else if (result == u8"quit") {
        return Options::QUIT;
    } else {
        b::log::error("Invalid option: {}\n", b::u8_as_str(result));
        return showOptions();
    }
}

void exec(bool save, bool multiple, bool folder) {
    b::filedialog dialog;
    dialog.directory = folder;

    if (!save) {
        if (multiple) {
            auto result = dialog.sync_open_multiple();
            for (auto& path : result) {
                b::log::debug("Selected file: {}", b::u8_as_str(path.u8string()));
            }
        }
        else {
            auto result = dialog.sync_open();
            b::log::debug("Selected file: {}", b::u8_as_str(result.u8string()));
        }
    }
    else {
        auto result = dialog.sync_save();
        b::log::debug("Selected file: {}", b::u8_as_str(result.u8string()));
    }
}

void show() {
    switch(showOptions()) {
        case Options::OPEN_FILE: exec(false, false, false); break;
        case Options::OPEN_FILE_MULTIPLE: exec(false, true, false); break;
        case Options::OPEN_FOLDER: exec(false, false, true); break;
        case Options::OPEN_FOLDER_MULTIPLE: exec(false, true, true); break;
        case Options::SAVE_FILE: exec(true, false, false); break;
        case Options::QUIT: return;
    }
    show();
}

int b::main(const std::vector<std::u8string>& args) {

    b::log::info("Running FileDialog example. Have fun!");
    show();
    b::log::info("Exiting FileDialog example. Bye!");

    return 0;
}
