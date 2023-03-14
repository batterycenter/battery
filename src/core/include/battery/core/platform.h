#pragma once

#include "battery/core/environment.h"
#include <string>

namespace battery {

    std::string get_config_home_folder();
    std::string get_data_home_folder();
    std::string get_state_folder();
    std::string get_cache_folder();
    std::string get_documents_folder();
    std::string get_desktop_folder();
    std::string get_pictures_folder();
    std::string get_music_folder();
    std::string get_video_folder();
    std::string get_download_folder();
    std::string get_save_games1_folder();
    std::string get_save_games2_folder();

    // TODO: Allow to retrieve the error message in a specific language
#ifdef BATTERY_ARCH_WINDOWS
    std::string get_last_win32_error();       // This function is only available on windows, for writing the
                                              // windows-specific side of cross-platform functions
#endif

}
