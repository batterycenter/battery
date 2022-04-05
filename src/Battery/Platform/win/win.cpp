
#include "Battery/Platform/Platform.h"
#include "Battery/StringUtils.h"

namespace Battery {

	bool platform_ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force) {

		if (!FileExists(file))
			return false;

		PrepareDirectory(targetDirectory);

		std::string command = std::string("powershell Expand-Archive ") + file.c_str() + " " + targetDirectory.c_str();

		if (force)
			command += " -Force";

		auto ret = ExecuteShellCommandSilent(command, true);
		return ret.first && (ret.second == 0);
	}

    std::pair<bool, size_t> platform_ExecuteShellCommandSilent(const std::string& command, bool hidden) {

		std::wstring Lcommand = L"/c " + StringUtils::MultiByteToWideChar(command);

		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = L"cmd";
		ShExecInfo.lpParameters = Lcommand.c_str();
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = hidden ? SW_HIDE : SW_SHOW;
		ShExecInfo.hInstApp = NULL;

		if (!ShellExecuteEx(&ShExecInfo)) {
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
	
	ALLEGRO_FILE* platform_LoadEmbeddedResource(int id, const char* type) {
		HMODULE hMod = GetModuleHandle(NULL);
		if (hMod == nullptr) { LOG_CORE_WARN("{}: {}", __FUNCTION__, "Can't find resource id {}: Module Handle is null!", id); return nullptr; }

		HRSRC hRes = FindResource(hMod, MAKEINTRESOURCE(id), Battery::StringUtils::MultiByteToWideChar(type).c_str());
		if (hRes == nullptr) { LOG_CORE_WARN("{}: {}", __FUNCTION__, "Can't find resource id {}: No such resource!", id); return nullptr; }
		
		HGLOBAL hGlobal = LoadResource(hMod, hRes);
		if (hGlobal == nullptr) { LOG_CORE_WARN("{}: {}", __FUNCTION__, "Can't find resource id {}: Resource can't be loaded!", id); return nullptr; }

		void* data = LockResource(hGlobal);
		if (data == nullptr) { LOG_CORE_WARN("{}: {}", __FUNCTION__, "Can't find resource id {}: Resource can't be locked!", id); return nullptr; }

		size_t size = SizeofResource(hMod, hRes);
		if (size == 0) { LOG_CORE_WARN("{}: {}", __FUNCTION__, "Can't find resource id {}: Size of resource is invalid!", id); return nullptr; }

		ALLEGRO_FILE* file = al_open_memfile(data, size, "r");
		if (file == nullptr) { LOG_CORE_WARN("{}: {}", __FUNCTION__, "Can't find resource id {}: Cannot open Allegro memfile!", id); return nullptr; }

		return file;
	}
}
