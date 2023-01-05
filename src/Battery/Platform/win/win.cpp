
#ifdef _WIN32

#include "Battery/Core/Log.h"
#include "Battery/Platform/Platform.h"
#include "Battery/Utils/StringUtils.h"
#include "Battery/Utils/FileUtils.h"
#include "Battery/Core/Exception.h"

#include "windows.h"
#include "shellapi.h"

namespace Battery {

	void* platform_LockFileDescriptor(const OsString& filepath) {
		void* descriptor = ::CreateFileW(filepath.wstr().c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (descriptor == INVALID_HANDLE_VALUE) {
			throw Battery::LockfileUnavailableException();
		}
		return descriptor;
	}

	void platform_UnlockFileDescriptor(void* fileDescriptor) {
		::CloseHandle(fileDescriptor);
	}

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

    std::pair<bool, size_t> platform_ExecuteShellCommandSilent(const OsString& command, bool hidden) {

		SHELLEXECUTEINFOW ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = nullptr;
		ShExecInfo.lpVerb = nullptr;
		ShExecInfo.lpFile = L"cmd";
		ShExecInfo.lpParameters = command.wstr().c_str();
		ShExecInfo.lpDirectory = nullptr;
		ShExecInfo.nShow = hidden ? SW_HIDE : SW_SHOW;
		ShExecInfo.hInstApp = nullptr;

		if (!ShellExecuteExW(&ShExecInfo)) {
			return std::make_pair(false, GetLastError());
		}

		if (!ShExecInfo.hProcess) {
			return std::make_pair(false, 0);
		}

		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

		DWORD exitCode;
		GetExitCodeProcess(ShExecInfo.hProcess, &exitCode);

		return std::make_pair(true, exitCode);
	}
	
	std::vector<uint8_t> platform_LoadEmbeddedResource(int id, const OsString& type) {
		std::vector<uint8_t> buffer;

		HMODULE hMod = GetModuleHandleW(nullptr);   // TODO: Message format is wrong!!!
		if (hMod == nullptr) { Log::Core::Warn("{}: {}", __FUNCTION__, "Can't find resource id {}: Module Handle is null!", id); return buffer; }

		HRSRC hRes = FindResourceW(hMod, MAKEINTRESOURCEW(id), type.wstr().c_str());
		if (hRes == nullptr) { Log::Core::Warn("{}: {}", __FUNCTION__, "Can't find resource id {}: No such resource!", id); return buffer; }
		
		HGLOBAL hGlobal = LoadResource(hMod, hRes);
		if (hGlobal == nullptr) { Log::Core::Warn("{}: {}", __FUNCTION__, "Can't find resource id {}: Resource can't be loaded!", id); return buffer; }

		void* data = LockResource(hGlobal);
		if (data == nullptr) { Log::Core::Warn("{}: {}", __FUNCTION__, "Can't find resource id {}: Resource can't be locked!", id); return buffer; }

		size_t size = SizeofResource(hMod, hRes);
		if (size == 0) { Log::Core::Warn("{}: {}", __FUNCTION__, "Can't find resource id {}: Size of resource is invalid!", id); return buffer; }

		buffer.resize(size);
		memcpy(&buffer[0], data, size);
		
		return buffer;
	}



#ifdef BATTERY_FEATURES_GRAPHICS
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
#endif // BATTERY_FEATURES_GRAPHICS









	static MB_Status win32_IDToEnum(int code) {
		if (code == IDOK) return MB_Status::OK;
		if (code == IDYES) return MB_Status::YES;
		if (code == IDNO) return MB_Status::NO;
		if (code == IDCANCEL) return MB_Status::CANCEL;
		if (code == IDABORT) return MB_Status::CANCEL;
		if (code == IDRETRY) return MB_Status::RETRY;
		if (code == IDTRYAGAIN) return MB_Status::RETRY;
		if (code == IDCONTINUE) return MB_Status::CONTINUE;
		if (code == IDIGNORE) return MB_Status::CONTINUE;
		return MB_Status::OK;
	}

	static int win32_BatteryEnumToMessageBoxEnum(MB_Buttons buttons) {
		if (buttons == MB_Buttons::OK) return MB_OK;
		if (buttons == MB_Buttons::OK_CANCEL) return MB_OKCANCEL;
		if (buttons == MB_Buttons::RETRY_CANCEL) return MB_RETRYCANCEL;
		if (buttons == MB_Buttons::YES_NO) return MB_YESNO;
		if (buttons == MB_Buttons::YES_NO_CANCEL) return MB_YESNOCANCEL;
		if (buttons == MB_Buttons::HELP) return MB_HELP;
		if (buttons == MB_Buttons::CANCEL_TRY_CONTINUE) return MB_CANCELTRYCONTINUE;
		if (buttons == MB_Buttons::ABORT_RETRY_IGNORE) return MB_ABORTRETRYIGNORE;
		return MB_OK;
	}

	MB_Status MessageBoxError(const OsString& message, const OsString& title, MB_Buttons buttons, int defaultButton) {
		int code = ::MessageBoxW(nullptr, message.wstr().c_str(), title.wstr().c_str(),
			MB_ICONERROR | win32_BatteryEnumToMessageBoxEnum(buttons) | ((defaultButton - 1) * 256));
		return win32_IDToEnum(code);
	}

	MB_Status MessageBoxWarning(const OsString& message, const OsString& title, MB_Buttons buttons, int defaultButton) {
		int code = ::MessageBoxW(nullptr, message.wstr().c_str(), title.wstr().c_str(),
			MB_ICONWARNING | win32_BatteryEnumToMessageBoxEnum(buttons) | ((defaultButton - 1) * 256));
		return win32_IDToEnum(code);
	}

	MB_Status MessageBoxInfo(const OsString& message, const OsString& title, MB_Buttons buttons, int defaultButton) {
		int code = ::MessageBoxW(nullptr, message.wstr().c_str(), title.wstr().c_str(),
			MB_ICONINFORMATION | win32_BatteryEnumToMessageBoxEnum(buttons) | ((defaultButton - 1) * 256));
		return win32_IDToEnum(code);
	}

	glm::vec2 GetUsableDesktopArea() {
		RECT xy;
		BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA, 0, &xy, 0);
		return { xy.right - xy.left, xy.bottom - xy.top };
	}

    std::string GetLastWin32ErrorString() {

        DWORD errorMessageID = ::GetLastError();
        if (errorMessageID == 0)
            return "";

        LPWSTR messageBuffer = nullptr;
        size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                     nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, nullptr);

        std::wstring message(messageBuffer, size);		// TODO: Check if buffer is still alive or if string is copied
        LocalFree(messageBuffer);

        return OsString(message);
    }

}

#endif // _WIN32
