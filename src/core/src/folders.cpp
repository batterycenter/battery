
#include "battery/folders.hpp"
#include "battery/extern/platform_folders.h"
#include "battery/internal/windows.hpp"

namespace b {

    static std::string& AppName() {
        static std::string appname = b::Constants::DefaultApplicationName();
        return appname;
    }

    std::string Folders::ApplicationName() {
        return AppName();
    }

    void Folders::SetApplicationName(const std::string& name) {
        AppName() = name;
    }

    fs::path Folders::ExecutablePath() {
#ifdef B_OS_WINDOWS
        std::array<wchar_t, static_cast<size_t>(MAX_PATH * 5)> buffer = {};
        if (GetModuleFileNameW(nullptr, buffer.data(), buffer.size()) == 0) {
            return {};
        }
        return b::narrow(buffer.data());
#elif defined(B_OS_WEB)
        return b::Folders::ApplicationName();
#else
        std::array<char, PATH_MAX> buffer = {};
        auto count = readlink("/proc/self/exe", buffer.data(), buffer.size());
        if (count == -1) {
            return {};
        }
        return std::string(buffer.data(), count);
#endif
    }

    fs::path Folders::ExecutableDir() {
        auto exe = ExecutablePath();
        return exe.parent_path();
    }




    fs::path Folders::AppHomeDir() {
        return SystemHomeDir() / ApplicationName();
    }

    fs::path Folders::AppConfigDir() {
        return SystemConfigDir() / ApplicationName();
    }

    fs::path Folders::AppLocalShareDir() {
        return SystemLocalShareDir() / ApplicationName();
    }

    fs::path Folders::AppCacheDir() {
        return SystemCacheDir() / ApplicationName();
    }

    fs::path Folders::AppRootDir() {
        return SystemAppRootDir() / ApplicationName();
    }

    fs::path Folders::AppDocumentsDir() {
        return SystemDocumentsDir() / ApplicationName();
    }

    fs::path Folders::AppDesktopDir() {
        return SystemDesktopDir() / ApplicationName();
    }

    fs::path Folders::AppPicturesDir() {
        return SystemPicturesDir() / ApplicationName();
    }

    fs::path Folders::AppMusicDir() {
        return SystemMusicDir() / ApplicationName();
    }

    fs::path Folders::AppVideoDir() {
        return SystemVideoDir() / ApplicationName();
    }

    fs::path Folders::AppDownloadsDir() {
        return SystemDownloadsDir() / ApplicationName();
    }





    fs::path Folders::SystemHomeDir() {
        return sago::getUserHome();
    }

    fs::path Folders::SystemConfigDir() {
        return sago::getConfigHome();
    }

    fs::path Folders::SystemLocalShareDir() {
        return sago::getDataHome();
    }

    fs::path Folders::SystemCacheDir() {
        return sago::getCacheDir();
    }

    fs::path Folders::SystemAppRootDir() {
        if (b::Constants::Platform() == b::Platform::Windows) {
            return sago::getStateDir();     // Local AppData
        }
        else if (b::Constants::Platform() == b::Platform::MacOS) {
            return SystemHomeDir() / "bin";     // ~/bin
        }
        else {
            return SystemHomeDir() / "bin";     // ~/bin
        }
    }

    fs::path Folders::SystemDocumentsDir() {
        return sago::getDocumentsFolder();
    }

    fs::path Folders::SystemDesktopDir() {
        return sago::getDesktopFolder();
    }

    fs::path Folders::SystemPicturesDir() {
        return sago::getPicturesFolder();
    }

    fs::path Folders::SystemMusicDir() {
        return sago::getMusicFolder();
    }

    fs::path Folders::SystemVideoDir() {
        return sago::getVideoFolder();
    }

    fs::path Folders::SystemDownloadsDir() {
        return sago::getDownloadFolder();
    }

} // namespace b
