#pragma once

#ifdef _WIN32
#include "Battery/Platform/win/win.h"
#else
#include "Battery/Platform/x11/x11.h"
#endif

struct ALLEGRO_FILE;

namespace Battery {

	bool platform_ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force);

    std::pair<bool, size_t> platform_ExecuteShellCommandSilent(const std::string& command, bool hidden);
	
	ALLEGRO_FILE* platform_LoadEmbeddedResource(int id, const char* type);

}
