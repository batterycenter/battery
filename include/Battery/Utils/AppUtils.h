#pragma once

#include "Battery/pch.h"

namespace Battery {
	
	//
	//	Explanation of Windows' appdata folders:
	//  In the %appdata% folder there is the Local and the Roaming folder.
	//  Local must be everything that stays on this computer and the roaming folder
	//  contains data that can move to another computer. Apparently the Roaming folder
	//  can be synchronized when you are on a domain.
	//


	
	
	///////////////////////////////////////////////////////////////
	/// \brief App data directory: This is where you save local data
	/// about your application, that can be shared across several computers.
	/// The Windows Roaming folder for example is synchronized in a domain.
	/// On Windows this is <roaming_appdata>/<app_name>, 
	/// on Linux this defaults to ~/.local/share/<app_name>.
	/// The folder may not exist.
	///////////////////////////////////////////////////////////////
	std::string GetAppDataDir();

	///////////////////////////////////////////////////////////////
	/// \brief Raw app data directory: Do not write here directly. 
	/// On Windows this is <roaming_appdata>, 
	/// on Linux this defaults to ~/.local/share.
	/// The folder may not exist.
	///////////////////////////////////////////////////////////////
	std::string GetRawAppDataDir();

	///////////////////////////////////////////////////////////////
	/// \brief Raw local app data directory: Do not write here directly.
	/// On Windows this is <local_appdata>, 
	/// on Linux this defaults to ~/.local   
	/// The folder may not exist.
	///////////////////////////////////////////////////////////////
	std::string GetRawLocalAppDataDir();

	///////////////////////////////////////////////////////////////
	/// \brief App config directory: This is where you save config data
	/// about your application, that is local to this computer and can't be shared.
	/// On Windows this is <local_appdata>/<app_name>/config, 
	/// on Linux this defaults to ~/.config/<app_name>.
	/// The folder may not exist.
	///////////////////////////////////////////////////////////////
	std::string GetConfigDir();

	///////////////////////////////////////////////////////////////
	/// \brief Local install dir: This is your local folder for installing 
	/// your application to, per-user and without admin rights. 
	/// On Windows this is <local_appdata>/<app_name>, 
	/// on Linux this defaults to ~/.local/<app_name>.
	/// The folder may not exist.
	///////////////////////////////////////////////////////////////
	std::string GetLocalInstallDir();

	///////////////////////////////////////////////////////////////
	/// \brief Cache dir: This is for any files you need temporarily, 
	/// but can be deleted later
	/// On Windows this is <local_appdata>/<app_name>/cache, 
	/// on Linux this defaults to ~/.cache/<app_name>.
	/// The folder may not exist.
	///////////////////////////////////////////////////////////////
	std::string GetCacheDir();

	///////////////////////////////////////////////////////////////
	/// \brief Startup dir: This is the folder used for starting 
	/// applications at boot. Create shortcuts here to start your app
	/// on bootup. 
	/// On Windows this is <roaming_appdata>/Microsoft/Windows/Start Menu/Programs/Startup, 
	/// on Linux this is /etc/init.d  
	/// However, the linux path is not guaranteed to be the same
	/// on all platforms. The folder may not exist.
	///////////////////////////////////////////////////////////////
	std::string GetStartupDir();

	///////////////////////////////////////////////
	/// \brief Desktop folder provided by the OS.
	/// The folder may not exist.
	///////////////////////////////////////////////
	std::string GetDesktopFolder();

	///////////////////////////////////////////////
	/// \brief Documents folder provided by the OS.
	/// The folder may not exist.
	///////////////////////////////////////////////
	std::string GetDocumentsFolder();

	///////////////////////////////////////////////
	/// \brief Download folder provided by the OS.
	/// The folder may not exist.
	///////////////////////////////////////////////
	std::string GetDownloadFolder();

	///////////////////////////////////////////////
	/// \brief Pictures folder provided by the OS.
	/// The folder may not exist.
	///////////////////////////////////////////////
	std::string GetPicturesFolder();

	///////////////////////////////////////////////
	/// \brief Public folder provided by the OS.
	/// The folder may not exist.
	///////////////////////////////////////////////
	std::string GetPublicFolder();

	///////////////////////////////////////////////
	/// \brief Music folder provided by the OS.
	/// The folder may not exist.
	///////////////////////////////////////////////
	std::string GetMusicFolder();

	///////////////////////////////////////////////
	/// \brief Video folder provided by the OS.
	/// The folder may not exist.
	///////////////////////////////////////////////
	std::string GetVideoFolder();
	
}