#pragma once

#ifdef _WIN32
#include "Battery/Platform/win/win.h"
#else
#include "Battery/Platform/x11/x11.h"
#endif

// Forward declarations for the Windows handle
struct HWND__;
typedef HWND__* HWND;
typedef unsigned long XID;

struct ALLEGRO_FILE;

namespace Battery {

	void* platform_LockFileDescriptor(const std::string& file);
	void platform_UnlockFileDescriptor(void* fileDescriptor);

	bool platform_ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force);

    std::pair<bool, size_t> platform_ExecuteShellCommandSilent(const std::string& command, bool hidden);
	
	ALLEGRO_FILE* platform_LoadEmbeddedResource(int id, const char* type);

	HWND platform_GetWinHandle(ALLEGRO_DISPLAY* allegroDisplayPointer);
	XID platform_GetWindowIdentifier(ALLEGRO_DISPLAY* allegroDisplayPointer);
	bool platform_IsFocused(ALLEGRO_DISPLAY* allegroDisplayPointer);
	void platform_Focus(ALLEGRO_DISPLAY* allegroDisplayPointer);
	void platform_Hide(ALLEGRO_DISPLAY* allegroDisplayPointer);
	void platform_Show(ALLEGRO_DISPLAY* allegroDisplayPointer);
	void platform_HideFromTaskbar(ALLEGRO_DISPLAY* allegroDisplayPointer);
	void platform_ShowInTaskbar(ALLEGRO_DISPLAY* allegroDisplayPointer);

}
