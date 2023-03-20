
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

	bool ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force) {
		return platform_ExtractArchive(file, targetDirectory, force);
	}

	std::vector<uint8_t> LoadEmbeddedResource(int id, const char* type) {
		return platform_LoadEmbeddedResource(id, type);
	}

}
