#pragma once

#ifdef BATTERY_ARCH_WINDOWS
#include "Battery/Platform/win/win.h"
#else
#include "Battery/Platform/x11/x11.h"
#endif

#include "Battery/common.h"
#include "Battery/Utils/OsString.h"

#ifdef BATTERY_FEATURES_GRAPHICS
#include "SFML/System.hpp"
#include "Battery/Platform/TrayIcon.h"
#endif // BATTERY_FEATURES_GRAPHICS

namespace Battery {

	void* platform_LockFileDescriptor(const OsString& filepath);
	void platform_UnlockFileDescriptor(void* fileDescriptor);

	bool platform_ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force);

    std::pair<bool, size_t> platform_ExecuteShellCommandSilent(const OsString& command, bool hidden);
	
	std::vector<uint8_t> platform_LoadEmbeddedResource(int id, const OsString& type);

#ifdef BATTERY_FEATURES_GRAPHICS
	bool platform_IsFocused(sf::WindowHandle window);
	void platform_Focus(sf::WindowHandle window);
	void platform_Hide(sf::WindowHandle window);
	void platform_Show(sf::WindowHandle window);
	void platform_HideFromTaskbar(sf::WindowHandle window);
	void platform_ShowInTaskbar(sf::WindowHandle window);
	void platform_SetWindowTransparent(sf::WindowHandle window, bool transparent);
	void platform_SetWindowAlpha(sf::WindowHandle window, uint8_t alpha);
#endif // BATTERY_FEATURES_GRAPHICS

	glm::vec2 GetUsableDesktopArea();

}
