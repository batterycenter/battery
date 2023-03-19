#pragma once

#include <string>
#include "battery/core/fs.hpp"
#include "battery/core/constants.hpp"

namespace b {

    struct folders {
        static fs::path get_executable_path();
        static fs::path get_executable_dir();

        static fs::path get_global_config_home();
        static fs::path get_global_data_home();
        static fs::path get_global_state();
        static fs::path get_global_cache();
        static fs::path get_global_documents();
        static fs::path get_global_desktop();
        static fs::path get_global_pictures();
        static fs::path get_global_music();
        static fs::path get_global_video();
        static fs::path get_global_downloads();
        static fs::path get_global_save_games1();
        static fs::path get_global_save_games2();

        static fs::path get_config_home();
        static fs::path get_data_home();
        static fs::path get_state();
        static fs::path get_cache();
        static fs::path get_documents();
        static fs::path get_desktop();
        static fs::path get_pictures();
        static fs::path get_music();
        static fs::path get_video();
        static fs::path get_downloads();
        static fs::path get_save_games1();
        static fs::path get_save_games2();

        inline static void set_application_name(const std::string& name) {
            appname = name;
        }

        inline static std::string get_application_name() {
            return appname;
        }

    private:
        inline static std::string appname = b::constants::default_application_name();
    };

}
