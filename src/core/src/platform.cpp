
#include "battery/core/platform.hpp"
#include "battery/core/extern/platform_folders.h"

namespace battery {

    // TODO: Add feature to set the application name and concatenate it to the paths

    std::string get_config_home_folder() {
        return sago::getConfigHome();
    }

    std::string get_data_home_folder() {
        return sago::getDataHome();
    }

    std::string get_state_folder() {
        return sago::getStateDir();
    }

    std::string get_cache_folder() {
        return sago::getCacheDir();
    }

    std::string get_documents_folder() {
        return sago::getDocumentsFolder();
    }

    std::string get_desktop_folder() {
        return sago::getDesktopFolder();
    }

    std::string get_pictures_folder() {
        return sago::getPicturesFolder();
    }

    std::string get_music_folder() {
        return sago::getMusicFolder();
    }

    std::string get_video_folder() {
        return sago::getVideoFolder();
    }

    std::string get_download_folder() {
        return sago::getDownloadFolder();
    }

    std::string get_save_games1_folder() {
        return sago::getSaveGamesFolder1();
    }

    std::string get_save_games2_folder() {
        return sago::getSaveGamesFolder2();
    }

}
