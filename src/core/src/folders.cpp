
#include "battery/core/folders.hpp"
#include "battery/core/extern/platform_folders.h"
#include "battery/core/internal/windows.hpp"

namespace b {

    fs::path Folders::ExecutablePath() {
#ifdef B_OS_WINDOWS
        std::array<wchar_t, static_cast<size_t>(MAX_PATH * 5)> buffer = {};
        if (GetModuleFileNameW(nullptr, buffer.data(), buffer.size()) == 0) {
            return {};
        }
        return b::string::decode<b::enc::os_native>(std::wstring(buffer.data()));
#else
        std::array<char, PATH_MAX> buffer = {};
        auto count = readlink("/proc/self/exe", buffer.data(), buffer.size());
        if (count == -1) {
            return {};
        }
        return b::string::decode<b::enc::os_native>(std::string(buffer.data(), count));
#endif
    }

    fs::path Folders::ExecutableDir() {
        auto exe = ExecutablePath();
        return exe.parent_path();
    }




    fs::path Folders::AppHomeDir() {
        return SystemHomeDir() + ApplicationName();
    }

    fs::path Folders::AppConfigDir() {
        return SystemConfigDir() + ApplicationName();
    }

    fs::path Folders::AppLocalShareDir() {
        return SystemLocalShareDir() + ApplicationName();
    }

    fs::path Folders::AppCacheDir() {
        return SystemCacheDir() + ApplicationName();
    }

    fs::path Folders::AppRootDir() {
        return SystemAppRootDir() + ApplicationName();
    }

    fs::path Folders::AppDocumentsDir() {
        return SystemDocumentsDir() + ApplicationName();
    }

    fs::path Folders::AppDesktopDir() {
        return SystemDesktopDir() + ApplicationName();
    }

    fs::path Folders::AppPicturesDir() {
        return SystemPicturesDir() + ApplicationName();
    }

    fs::path Folders::AppMusicDir() {
        return SystemMusicDir() + ApplicationName();
    }

    fs::path Folders::AppVideoDir() {
        return SystemVideoDir() + ApplicationName();
    }

    fs::path Folders::AppDownloadsDir() {
        return SystemDownloadsDir() + ApplicationName();
    }





    fs::path Folders::SystemHomeDir() {
        return b::string::decode<b::enc::utf8>(sago::getUserHome());
    }

    fs::path Folders::SystemConfigDir() {
        return b::string::decode<b::enc::utf8>(sago::getConfigHome());
    }

    fs::path Folders::SystemLocalShareDir() {
        return b::string::decode<b::enc::utf8>(sago::getDataHome());
    }

    fs::path Folders::SystemCacheDir() {
        return b::string::decode<b::enc::utf8>(sago::getCacheDir());
    }

    fs::path Folders::SystemAppRootDir() {
        if (b::Constants::Platform() == b::Platform::Windows) {
            return b::string::decode<b::enc::utf8>(sago::getStateDir());     // Local AppData
        }
        else if (b::Constants::Platform() == b::Platform::MacOS) {
            return SystemHomeDir() / "bin"_b;     // ~/bin
        }
        else {
            return SystemHomeDir() / "bin"_b;     // ~/bin
        }
    }

    fs::path Folders::SystemDocumentsDir() {
        return b::string::decode<b::enc::utf8>(sago::getDocumentsFolder());
    }

    fs::path Folders::SystemDesktopDir() {
        return b::string::decode<b::enc::utf8>(sago::getDesktopFolder());
    }

    fs::path Folders::SystemPicturesDir() {
        return b::string::decode<b::enc::utf8>(sago::getPicturesFolder());
    }

    fs::path Folders::SystemMusicDir() {
        return b::string::decode<b::enc::utf8>(sago::getMusicFolder());
    }

    fs::path Folders::SystemVideoDir() {
        return b::string::decode<b::enc::utf8>(sago::getVideoFolder());
    }

    fs::path Folders::SystemDownloadsDir() {
        return b::string::decode<b::enc::utf8>(sago::getDownloadFolder());
    }

} // namespace b
