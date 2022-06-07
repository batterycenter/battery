#pragma once

#ifdef _WIN32
#include "Battery/Platform/win/win.h"
#else
#include "Battery/Platform/x11/x11.h"
#endif

#include "SFML/System.hpp"

namespace Battery {

	void* platform_LockFileDescriptor(const std::string& file);
	void platform_UnlockFileDescriptor(void* fileDescriptor);

	bool platform_ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force);

    std::pair<bool, size_t> platform_ExecuteShellCommandSilent(const std::string& command, bool hidden);
	
	std::vector<uint8_t> platform_LoadEmbeddedResource(int id, const char* type);

	bool platform_IsFocused(sf::WindowHandle window);
	void platform_Focus(sf::WindowHandle window);
	void platform_Hide(sf::WindowHandle window);
	void platform_Show(sf::WindowHandle window);
	void platform_HideFromTaskbar(sf::WindowHandle window);
	void platform_ShowInTaskbar(sf::WindowHandle window);

	std::string platform_GetLastWin32ErrorAsString();

}
