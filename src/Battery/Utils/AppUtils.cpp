
#include "Battery/Core/Application.h"
#include "Battery/Utils/AppUtils.h"
#include "Battery/Utils/FileUtils.h"
#include "Battery/Extern/PlatformFolders.h"

#ifdef _WIN32
#define WINDOWS		// Linux = Non-Windows
#endif

namespace Battery {
	
	std::string GetAppDataDir() {
		return GetRawAppDataDir() + "/" + Battery::GetApp().applicationName;
	}
	
	std::string GetRawAppDataDir() {
		return sago::getDataHome();
	}

	std::string GetRawLocalAppDataDir() {
#ifdef WINDOWS
		return sago::getCacheDir();
#else
		return GetParentDirectory(sago::getDataHome());
#endif
	}

	std::string GetConfigDir() {
#ifdef WINDOWS
		return GetLocalInstallDir() + "/config";
#else
		return sago::getConfigHome() + "/" + Battery::GetApp().applicationName;
#endif
	}

	std::string GetLocalInstallDir() {
		return GetRawLocalAppDataDir() + "/" + Battery::GetApp().applicationName;
	}
	
	std::string GetCacheDir() {
#ifdef WINDOWS
		return GetLocalInstallDir() + "/cache";
#else
		return sago::getCacheDir() + "/" + Battery::GetApp().applicationName;
#endif
	}

	std::string GetStartupDir() {
#ifdef WINDOWS
		return GetRawAppDataDir() + "/Microsoft/Windows/Start Menu/Programs/Startup";
#else
		return "/etc/init.d";
#endif
	}

	std::string GetDesktopFolder() {
		return sago::getDesktopFolder();
	}

	std::string GetDocumentsFolder() {
		return sago::getDocumentsFolder();
	}

	std::string GetDownloadFolder() {
		return sago::getDownloadFolder();
	}

	std::string GetPicturesFolder() {
		return sago::getPicturesFolder();
	}

	std::string GetPublicFolder() {
		return sago::getPublicFolder();
	}

	std::string GetMusicFolder() {
		return sago::getMusicFolder();
	}

	std::string GetVideoFolder() {
		return sago::getVideoFolder();
	}
	
}
