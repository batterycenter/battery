#pragma once

#include <string>
#include "fs.hpp"
#include "constants.hpp"

namespace b {

    // We use static functions instead of static global variables to have defined lifetime, the ability
    // to catch first-time constructor exceptions and avoid static initialization order fiascos
    // https://isocpp.org/wiki/faq/ctors#static-init-order
    namespace Folders {
        fs::path ExecutablePath();
        fs::path ExecutableDir();

        // WARNING: These paths are not guaranteed to exist, use fs::create_directories() to create them
        // WARNING: These paths partly overlap on certain systems, be careful with filenames!

        fs::path AppHomeDir();               // This application's subdirectory inside the system-wide user's home directory                                         -> <SystemHomeDir()>/<ApplicationName()>
        fs::path AppConfigDir();             // This application's subdirectory for plain-text configuration files that may be edited by the user                    -> <SystemConfigDir()>/<ApplicationName()>
        fs::path AppLocalShareDir();         // This application's subdirectory for intermediate resource files that are not strictly needed for the app to work     -> <SystemLocalShareDir()>/<ApplicationName()>
        fs::path AppCacheDir();              // This application's subdirectory for temporarily cached files, which can safely be deleted by the user                -> <SystemCacheDir()>/<ApplicationName()>
        fs::path AppRootDir();               // This application's root directory, including its own optimal install location                                        -> <SystemAppRootDir()>/<ApplicationName()>
        fs::path AppDocumentsDir();          // This application's subdirectory for user documents                                                                   -> <SystemDocumentsDir()>/<ApplicationName()>
        fs::path AppDesktopDir();            // This application's subdirectory for user desktop files                                                               -> <SystemDesktopDir()>/<ApplicationName()>
        fs::path AppPicturesDir();           // This application's subdirectory for user pictures                                                                    -> <SystemPicturesDir()>/<ApplicationName()>
        fs::path AppMusicDir();              // This application's subdirectory for user music                                                                       -> <SystemMusicDir()>/<ApplicationName()>
        fs::path AppVideoDir();              // This application's subdirectory for user videos                                                                      -> <SystemVideoDir()>/<ApplicationName()>
        fs::path AppDownloadsDir();          // This application's subdirectory for user downloaded files                                                            -> <SystemDownloadsDir()>/<ApplicationName()>

        fs::path SystemHomeDir();            // System-wide user's home directory                                                                            (Defaults -> Win: %USERPROFILE%, Linux & Mac: $HOME)
        fs::path SystemConfigDir();          // System-wide directory for plain-text configuration files that may be edited by the user                      (Defaults -> Win: %APPDATA%, Linux: ~/.config, Mac: ~/Library/Application Support)
        fs::path SystemLocalShareDir();      // System-wide directory for intermediate application files that are not strictly needed for the app to work    (Defaults -> Win: %APPDATA%, Linux: ~/.local/share, Mac: ~/Library/Application Support)
        fs::path SystemCacheDir();           // System-wide directory for temporarily cached files, which can safely be deleted by the user                  (Defaults -> Win: %APPDATA%, Linux: ~/.cache, Mac: ~/Library/Caches)
        fs::path SystemAppRootDir();         // System-wide directory for installing the application files per-user                                          (Defaults -> Win: %LOCALAPPDATA%, Linux: ~/bin, Mac: ~/bin)
        fs::path SystemDocumentsDir();       // System-wide directory for user documents                                                                     (Defaults -> Win: %USERPROFILE%/Documents, Linux & Mac: ~/Documents)
        fs::path SystemDesktopDir();         // System-wide directory for user desktop files                                                                 (Defaults -> Win: %USERPROFILE%/Desktop, Linux & Mac: ~Desktop)
        fs::path SystemPicturesDir();        // System-wide directory for user pictures                                                                      (Defaults -> Win: %USERPROFILE%/Pictures, Linux & Mac: ~/Pictures)
        fs::path SystemMusicDir();           // System-wide directory for user music                                                                         (Defaults -> Win: %USERPROFILE%/Music, Linux & Mac: ~/Music)
        fs::path SystemVideoDir();           // System-wide directory for user videos                                                                        (Defaults -> Win: %USERPROFILE%/Videos, Linux & Mac: ~/Videos)
        fs::path SystemDownloadsDir();       // System-wide directory for user downloaded files                                                              (Defaults -> Win: %USERPROFILE%/Downloads, Linux & Mac: ~/Downloads)

        inline static std::string& ApplicationName() {
            static std::string appname = b::Constants::DefaultApplicationName();
            return appname;
        }
    };

}
