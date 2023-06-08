
#ifdef _WIN32

#include "Battery/Core/log.hpp"
#include "Battery/Platform/Platform.h"
#include "Battery/Utils/StringUtils.h"
#include "Battery/Utils/FileUtils.h"
#include "Battery/Core/Exception.h"

#include "windows.h"
#include "shellapi.h"

namespace Battery {

	bool platform_ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force) {

		if (!FS::exists(file))
			return false;

		FS::create_directories(targetDirectory);

		std::string command = std::string("powershell Expand-Archive ") + file.c_str() + " " + targetDirectory.c_str();

		if (force)
			command += " -Force";

		auto[success, returnCode] = FS::ExecuteShellCommandSilent(command, true);
		return success && returnCode == 0;
	}



#ifdef B_MODULE_LOADED_graphics
	bool platform_IsFocused(sf::WindowHandle window) {
		return GetForegroundWindow() == window;
	}

	void platform_Focus(sf::WindowHandle window) {
		SetForegroundWindow(window);
	}

	void platform_Hide(sf::WindowHandle window) {
		ShowWindow(window, SW_HIDE);
	}

	void platform_Show(sf::WindowHandle window) {
		ShowWindow(window, SW_SHOW);
	}

	void platform_HideFromTaskbar(sf::WindowHandle window) {
		long style = GetWindowLongW(window, GWL_STYLE);
		style |= WS_VISIBLE;
		ShowWindow(window, SW_HIDE);
		SetWindowLongW(window, GWL_STYLE, style);
		ShowWindow(window, SW_SHOW);
	}

	void platform_ShowInTaskbar(sf::WindowHandle window) {
		long style = GetWindowLongW(window, GWL_STYLE);
		style &= ~(WS_VISIBLE);
		SetWindowLongW(window, GWL_STYLE, style);
		ShowWindow(window, SW_SHOW);
	}

	void platform_SetWindowTransparent(sf::WindowHandle window, bool transparent) {
		if (transparent) {
			SetWindowLongPtrW(window, GWL_EXSTYLE, GetWindowLongPtr(window, GWL_EXSTYLE) | WS_EX_LAYERED);
		}
		else {
			SetWindowLongPtrW(window, GWL_EXSTYLE, GetWindowLongPtr(window, GWL_EXSTYLE) & ~WS_EX_LAYERED);
			RedrawWindow(window, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
		}
	}

	void platform_SetWindowAlpha(sf::WindowHandle window, uint8_t alpha) {
		SetLayeredWindowAttributes(window, 0, alpha, LWA_ALPHA);
	}
#endif









	glm::vec2 GetUsableDesktopArea() {
		RECT xy;
		BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA, 0, &xy, 0);
		return { xy.right - xy.left, xy.bottom - xy.top };
	}

}

#endif // _WIN32
