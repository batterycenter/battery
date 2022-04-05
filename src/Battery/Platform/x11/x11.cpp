
#include "Battery/Platform/Platform.h"
#include "Battery/StringUtils.h"

namespace Battery {

	bool platform_ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force) {
#warning TODO: Implement unzipping
		return false;
	}

    std::pair<bool, size_t> platform_ExecuteShellCommandSilent(const std::string& command, bool hidden) {
		throw Battery::NotImplementedException();
	}
	
	ALLEGRO_FILE* platform_LoadEmbeddedResource(int id, const char* type) {
		throw Battery::NotImplementedException();
	}
}
