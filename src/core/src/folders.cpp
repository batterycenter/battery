
#include "battery/core/environment.hpp"
#include "battery/core/folders.hpp"
#include "battery/core/extern/platform_folders.h"
#include "battery/core/internal/windows.hpp"

namespace b {

    fs::path folders::get_executable_path() {
#ifdef BATTERY_ARCH_WINDOWS
        wchar_t buffer[MAX_PATH * 5];
        if (GetModuleFileNameW(nullptr, buffer, sizeof(buffer)) == 0) {
            return {};
        }
        return b::osstring_to_u8(buffer);
#else
        char buffer[PATH_MAX];
        auto count = readlink("/proc/self/exe", buffer, sizeof(buffer));
        if (count == -1) {
            return {};
        }
        return std::u8string(buffer, count);
#endif
    }

    fs::path folders::get_executable_dir() {
        auto exe = get_executable_path();
        return exe.parent_path();
    }



    fs::path folders::get_global_config_home() {
        return sago::getConfigHome();
    }

    fs::path folders::get_global_data_home() {
        return sago::getDataHome();
    }

    fs::path folders::get_global_state() {
        return sago::getStateDir();
    }

    fs::path folders::get_global_cache() {
        return sago::getCacheDir();
    }

    fs::path folders::get_global_documents() {
        return sago::getDocumentsFolder();
    }

    fs::path folders::get_global_desktop() {
        return sago::getDesktopFolder();
    }

    fs::path folders::get_global_pictures() {
        return sago::getPicturesFolder();
    }

    fs::path folders::get_global_music() {
        return sago::getMusicFolder();
    }

    fs::path folders::get_global_video() {
        return sago::getVideoFolder();
    }

    fs::path folders::get_global_downloads() {
        return sago::getDownloadFolder();
    }

    fs::path folders::get_global_save_games1() {
        return sago::getSaveGamesFolder1();
    }

    fs::path folders::get_global_save_games2() {
        return sago::getSaveGamesFolder2();
    }




    fs::path folders::get_config_home() {
        return get_global_config_home() + appname;
    }

    fs::path folders::get_data_home() {
        return get_global_data_home() + appname;
    }

    fs::path folders::get_state() {
        return get_global_state() + appname;
    }

    fs::path folders::get_cache() {
        return get_global_cache() + appname;
    }

    fs::path folders::get_documents() {
        return get_global_documents() + appname;
    }

    fs::path folders::get_desktop() {
        return get_global_desktop() + appname;
    }

    fs::path folders::get_pictures() {
        return get_global_pictures() + appname;
    }

    fs::path folders::get_music() {
        return get_global_music() + appname;
    }

    fs::path folders::get_video() {
        return get_global_video() + appname;
    }

    fs::path folders::get_downloads() {
        return get_global_downloads() + appname;
    }

    fs::path folders::get_save_games1() {
        return get_global_save_games1() + appname;
    }

    fs::path folders::get_save_games2() {
        return get_global_save_games2() + appname;
    }

} // namespace b
