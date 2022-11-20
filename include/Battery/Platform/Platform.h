#pragma once

#ifdef _WIN32
#include "Battery/Platform/win/win.h"
#else
#include "Battery/Platform/x11/x11.h"
#endif

#include "SFML/System.hpp"
#include "Battery/Platform/TrayIcon.h"
#include "Battery/Utils/OsString.h"

namespace Battery {

	void* platform_LockFileDescriptor(const OsString& filepath);
	void platform_UnlockFileDescriptor(void* fileDescriptor);

	bool platform_ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force);

    std::pair<bool, size_t> platform_ExecuteShellCommandSilent(const OsString& command, bool hidden);
	
	std::vector<uint8_t> platform_LoadEmbeddedResource(int id, const OsString& type);

	bool platform_IsFocused(sf::WindowHandle window);
	void platform_Focus(sf::WindowHandle window);
	void platform_Hide(sf::WindowHandle window);
	void platform_Show(sf::WindowHandle window);
	void platform_HideFromTaskbar(sf::WindowHandle window);
	void platform_ShowInTaskbar(sf::WindowHandle window);
	void platform_SetWindowTransparent(sf::WindowHandle window, bool transparent);
	void platform_SetWindowAlpha(sf::WindowHandle window, uint8_t alpha);

	enum class MB_Buttons {
		OK,
		OK_CANCEL,
		RETRY_CANCEL,
		YES_NO,
		YES_NO_CANCEL,
		HELP,
		CANCEL_TRY_CONTINUE,
		ABORT_RETRY_IGNORE
	};

	enum class MB_Status {
		OK,
		YES,
		NO,
		CANCEL,			// also contains ABORT
		RETRY,			// also contains TRY_AGAIN
		CONTINUE		// also contains IGNORE
	};

	// defaultButton: 1, 2, 3 or 4
	MB_Status MessageBoxError(const OsString& message, const OsString& title = "Error", MB_Buttons buttons = MB_Buttons::OK, int defaultButton = 1);
	MB_Status MessageBoxWarning(const OsString& message, const OsString& title = "Warning", MB_Buttons buttons = MB_Buttons::OK, int defaultButton = 1);
	MB_Status MessageBoxInfo(const OsString& message, const OsString& title = "Information", MB_Buttons buttons = MB_Buttons::OK, int defaultButton = 1);

	glm::vec2 GetUsableDesktopArea();

#ifdef _WIN32
    std::string GetLastWin32ErrorString();
#endif
}
