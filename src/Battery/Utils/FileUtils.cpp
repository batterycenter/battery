
#include "Battery/Core/Exception.h"
#include "Battery/Core/Config.h"
#include "Battery/Core/log.hpp"
#include "Battery/Utils/FileUtils.h"
#include "Battery/Platform/Platform.h"

#include "cpplocate/cpplocate.h"
#include <filesystem>
#include <sys/stat.h>
namespace fs = std::filesystem;

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

#ifdef MoveFile
#undef MoveFile
#endif

#ifdef WriteFile
#undef WriteFile
#endif

#ifdef CopyFile
#undef CopyFile
#endif

#ifdef RemoveDirectory
#undef RemoveDirectory
#endif

namespace Battery::FS {
	
	Lockfile::Lockfile(const std::string& filename, bool createDirectories) {
		if (createDirectories) {
			FS::ofstream file(filename);	// TODO: Check this
		}
		fileDescriptor = platform_LockFileDescriptor(filename);
	}

	Lockfile::~Lockfile() {
		platform_UnlockFileDescriptor(fileDescriptor);
	}











	std::string GetExecutablePath() {
		return cpplocate::getExecutablePath();
	}

	std::string GetExecutableName() {
		return GetFilename(GetExecutablePath());
	}

	std::string GetExecutableDirectory() {
		return GetParentDirectory(GetExecutablePath());
	}








	std::string GetFilename(const std::string& path) {
		return fs::path(path).filename().string();
	}

	std::string GetBasename(const std::string& path) {
		return fs::path(path).replace_extension().string();
	}

	std::string GetExtension(const std::string& path) {
		if (fs::path(path).has_extension())
			return fs::path(path).extension().string();
		else
			return "";
	}

	std::string GetParentDirectory(const std::string& path) {
		return fs::path(path).parent_path().string();
	}












	std::pair<bool, size_t> ExecuteShellCommand(const std::string& command) {
		int exitCode = system(command.c_str());
		return std::make_pair(exitCode == 0, exitCode);
	}

	std::pair<bool, size_t> ExecuteShellCommandSilent(const std::string& command, bool hidden) {
		return platform_ExecuteShellCommandSilent(command, hidden);
	}







	bool ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force) {
		return platform_ExtractArchive(file, targetDirectory, force);
	}

	std::vector<uint8_t> LoadEmbeddedResource(int id, const char* type) {
		return platform_LoadEmbeddedResource(id, type);
	}

}
