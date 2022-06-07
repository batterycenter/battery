
#ifdef _WIN32

#include "Battery/Core/Log.h"
#include "Battery/Platform/Platform.h"
#include "Battery/Utils/StringUtils.h"
#include "Battery/Utils/FileUtils.h"
#include "Battery/Core/Exception.h"

#include "windows.h"
#include "shellapi.h"

namespace Battery {

	void* platform_LockFileDescriptor(const std::string& file) {
		std::wstring wide = MultiByteToWideChar(file);
		void* descriptor = ::CreateFileW(wide.c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (descriptor == INVALID_HANDLE_VALUE) {
			throw Battery::LockfileUnavailableException();
		}
		return descriptor;
	}

	void platform_UnlockFileDescriptor(void* fileDescriptor) {
		::CloseHandle(fileDescriptor);
	}

	bool platform_ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force) {

		if (!FileExists(file))
			return false;

		PrepareDirectory(targetDirectory);

		std::string command = std::string("powershell Expand-Archive ") + file.c_str() + " " + targetDirectory.c_str();

		if (force)
			command += " -Force";

		auto[success, returnCode] = ExecuteShellCommandSilent(command, true);
		return success && returnCode == 0;
	}

    std::pair<bool, size_t> platform_ExecuteShellCommandSilent(const std::string& command, bool hidden) {

		std::wstring Lcommand = L"/c " + MultiByteToWideChar(command);

		SHELLEXECUTEINFOW ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = L"cmd";
		ShExecInfo.lpParameters = Lcommand.c_str();
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = hidden ? SW_HIDE : SW_SHOW;
		ShExecInfo.hInstApp = NULL;

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
	
	std::vector<uint8_t> platform_LoadEmbeddedResource(int id, const char* type) {
		std::vector<uint8_t> buffer;

		HMODULE hMod = GetModuleHandleW(NULL);
		if (hMod == nullptr) { LOG_CORE_WARN("{}: {}", __FUNCTION__, "Can't find resource id {}: Module Handle is null!", id); return buffer; }

		HRSRC hRes = FindResourceW(hMod, MAKEINTRESOURCEW(id), Battery::MultiByteToWideChar(type).c_str());
		if (hRes == nullptr) { LOG_CORE_WARN("{}: {}", __FUNCTION__, "Can't find resource id {}: No such resource!", id); return buffer; }
		
		HGLOBAL hGlobal = LoadResource(hMod, hRes);
		if (hGlobal == nullptr) { LOG_CORE_WARN("{}: {}", __FUNCTION__, "Can't find resource id {}: Resource can't be loaded!", id); return buffer; }

		void* data = LockResource(hGlobal);
		if (data == nullptr) { LOG_CORE_WARN("{}: {}", __FUNCTION__, "Can't find resource id {}: Resource can't be locked!", id); return buffer; }

		size_t size = SizeofResource(hMod, hRes);
		if (size == 0) { LOG_CORE_WARN("{}: {}", __FUNCTION__, "Can't find resource id {}: Size of resource is invalid!", id); return buffer; }

		buffer.resize(size);
		memcpy(&buffer[0], data, size);
		
		return buffer;
	}

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

	std::string platform_GetLastWin32ErrorAsString() {

		DWORD errorMessageID = ::GetLastError();
		if (errorMessageID == 0)
			return "";

		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		std::string message(messageBuffer, size);
		LocalFree(messageBuffer);

		return message;
	}

}

#endif // _WIN32
