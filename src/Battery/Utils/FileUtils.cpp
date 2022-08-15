
#include "Battery/Core/Exception.h"
#include "Battery/Core/Config.h"
#include "Battery/Core/Log.h"
#include "Battery/Utils/FileUtils.h"
#include "Battery/Platform/Platform.h"

#include "cpplocate/cpplocate.h"
#include <filesystem>
namespace fs = std::filesystem;

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

#ifdef MoveFile
#undef MoveFile
#endif

#ifdef CopyFile
#undef CopyFile
#endif

#ifdef RemoveDirectory
#undef RemoveDirectory
#endif

#define CHECK_ALLEGRO_INIT() \
	if (!Battery::AllegroContext::GetInstance()->IsInitialized()) {	\
		throw Battery::Exception(std::string(__FUNCTION__)  + ": Allegro Context is not initialized!");	\
	}

namespace Battery {

	Lockfile::Lockfile(const std::string& file, bool createDirectories) {
		if (createDirectories) {
			Battery::WriteFile(file, "");
		}
		fileDescriptor = platform_LockFileDescriptor(file);
	}

	Lockfile::~Lockfile() {
		platform_UnlockFileDescriptor(fileDescriptor);
	}





	
	bool FilenameExists(const std::string& path) {
		return FileExists(path) || DirectoryExists(path);
	}

	bool FileExists(const std::string& path) {
  		struct stat buffer;   
  		return (stat (path.c_str(), &buffer) == 0); 
	}

	bool DirectoryExists(const std::string& path) {
		struct stat info;
    	if(stat(path.c_str(), &info) != 0) return 0;
		else if(info.st_mode & S_IFDIR) return 1;
		return 0;
	}

	std::vector<std::filesystem::path> GetDirectoryContent(const std::string& path) {
		std::vector<std::filesystem::path> elements;
		for (const auto& entry : std::filesystem::directory_iterator(path))
        	elements.push_back(entry.path());
		return elements;
	}

	bool MakeDirectory(const std::string& path) {
		return std::filesystem::create_directory(path);
	}

	bool RenameFile(const std::string& file, const std::string& targetFile) {
		try {
			std::filesystem::rename(file, targetFile);
			return true;
		}
		catch (std::filesystem::filesystem_error& e) {
			LOG_CORE_ERROR("Failed to rename file '{}' to '{}': {}", file, targetFile, e.what());
		}

		return false;
	}

	bool CopyFile(const std::string& sourceFile, std::string targetFile) {

		PrepareDirectory(GetParentDirectory(targetFile));

		try {
			std::filesystem::copy(sourceFile, targetFile);
			return true;
		}
		catch (std::filesystem::filesystem_error& e) {
			LOG_CORE_ERROR("{}: {}", __FUNCTION__, "Failed to copy file: {}", e.what());
		}

		return false;
	}

	bool MoveFile(const std::string& file, std::string directory) {

		PrepareDirectory(directory);
		std::string targetFile = directory + GetFilename(file);

		try {
			std::filesystem::rename(file, targetFile);
			return true;
		}
		catch (std::filesystem::filesystem_error& e) {
			LOG_CORE_ERROR("{}: {}", __FUNCTION__, "Failed to move file: {}", e.what());
		}

		return false;
	}

	bool MoveDirectory(std::string source, std::string target) {
		if (source.back() != '/') source += "/";
		if (target.back() != '/') target += "/";
		if (!DirectoryExists(source)) {
			LOG_CORE_ERROR("{}: {}", __FUNCTION__, "Source directory '{}' not available", source);
			return false;
		}

		PrepareDirectory(target);
		LOG_CORE_DEBUG("{}: {}", __FUNCTION__, "Moving directory '{}' to '{}' recursively", source, target);

		for (std::filesystem::path _entry : GetDirectoryContent(source)) {
			std::string entry = _entry.string();

			if (FileExists(source + entry)) {	// It's a file
				if (!MoveFile(source + entry, target)) {
					LOG_CORE_WARN("{}: {}", __FUNCTION__, "File '{}' could not be moved", source + entry);
					return false;
				}
			}
			else {		// It's a directory
				if (!MoveDirectory(source + entry, target + entry)) {
					LOG_CORE_WARN("{}: {}", __FUNCTION__, "Directory '{}' could not be moved", source + entry);
					return false;
				}
			}
		}

		RemoveDirectory(source);

		LOG_CORE_TRACE("{}: {}", __FUNCTION__, "Done");
		return true;
	}

	std::tuple<bool, std::string> ReadFile(const std::string& path, bool binary) {

		std::ifstream file(path, binary ? std::ios::binary : std::ios::out);
		if (!file.is_open()) {
			return std::make_tuple<bool, std::string>(false, "");
		}
		
		std::stringstream buffer;
		buffer << file.rdbuf();

		file.close();

		return std::make_tuple<bool, std::string>(true, buffer.str());
	}

	bool WriteFile(const std::string& path, const std::string& content, bool binary) {

		PrepareDirectory(GetParentDirectory(path));

		std::ofstream file(path, binary ? std::ios::binary : std::ios::out);
		if (!file.is_open()) {
			return false;
		}

		file << content;

		file.close();

		return true;
	}

	bool RemoveFile(const std::string& path) {
		return std::remove(path.c_str()) == 0;
	}

	bool RemoveDirectory(const std::string& path) {

		if (!DirectoryExists(path)) {							// If directory can't be accessed, check if the parent can
			return FilenameExists(GetParentDirectory(path));	// be accessed to see if it's a permission problem
		}	// TODO: Check this, doesn't seem right

		for (auto _entry : GetDirectoryContent(path)) {
			std::string entry = _entry.string();
			if (DirectoryExists(path + "/" + entry)) {
				if (!RemoveDirectory(path + "/" + entry))
					return false;
			}
			else {
				if (!RemoveFile(path + "/" + entry))
					return false;
			}
		}

		return RemoveFile(path);
	}

	bool RemoveDirectoryContent(const std::string& path) {

		if (!DirectoryExists(path)) {
			return false;
		}

		for (auto _entry : GetDirectoryContent(path)) {
			std::string entry = _entry.string();
			if (DirectoryExists(path + "/" + entry)) {
				if (!RemoveDirectory(path + "/" + entry))
					return false;
			}
			else {
				if (!RemoveFile(path + "/" + entry))
					return false;
			}
		}

		return true;
	}

	bool PrepareEmptyDirectory(const std::string& path) {

		// First prepare a directory so it exists
		if (!PrepareDirectory(path))
			return false;

		// Then delete all content so it's nice and empty
		return RemoveDirectoryContent(path);
	}

	bool PrepareDirectory(const std::string& path) {

		if (DirectoryExists(path))
			return true;

		return MakeDirectory(path);
	}












	std::string GetExecutablePath() {
		return cpplocate::getExecutablePath();
	}

	std::string GetExecutableName() {
		return GetFilename(GetExecutablePath());
	}

	std::string GetExecutableDirectory() {
		return GetParentDirectory(GetExecutablePath());
	}








	std::string GetFilename(const std::string& path) {
		return fs::path(path).filename().string();
	}

	std::string GetBasename(const std::string& path) {
		return fs::path(path).replace_extension().string();
	}

	std::string GetExtension(const std::string& path) {
		if (fs::path(path).has_extension())
			return fs::path(path).extension().string();
		else
			return "";
	}

	std::string GetParentDirectory(const std::string& path) {
		return fs::path(path).parent_path().string();
	}












	std::pair<bool, size_t> ExecuteShellCommand(const std::string& command) {
		int exitCode = system(command.c_str());
		return std::make_pair(exitCode == 0, exitCode);
	}

	std::pair<bool, size_t> ExecuteShellCommandSilent(const std::string& command, bool hidden) {
		return platform_ExecuteShellCommandSilent(command, hidden);
	}







	bool ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force) {
		return platform_ExtractArchive(file, targetDirectory, force);
	}

	std::vector<uint8_t> LoadEmbeddedResource(int id, const char* type) {
		return platform_LoadEmbeddedResource(id, type);
	}

}
