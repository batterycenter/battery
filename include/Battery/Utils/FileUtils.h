#pragma once

#include "Battery/common.h"
#include "Battery/Platform/Platform.h"

#include <filesystem>

namespace Battery::FS {

	using std::filesystem::path;					// Path class for working with filenames

	using std::filesystem::exists;					// Checks if a path exists on-disk (file or directory)
	using std::filesystem::create_directory;		// Creates a single directory
	using std::filesystem::create_directories;		// Creates a directory and all above
	using std::filesystem::copy;					// Copy either a file or a directory

	using std::filesystem::is_regular_file;
	using std::filesystem::is_directory;

	enum class Mode {
		TEXT,
		BINARY
	};

	class ifstream : public std::ifstream {
	public:
		ifstream(const std::string& path, enum Mode filemode = Mode::TEXT)
			: std::ifstream(convert(path), 
				(filemode == Mode::TEXT) ? (std::ios::in) : (std::ios::in | std::ios::binary)) {
			this->path = path;
			binary = (filemode == Mode::BINARY);	// remember for later
		}
		
		ifstream(const std::string& path, std::ios_base::openmode mode)
			: std::ifstream(convert(path), mode) {
			this->path = path;
			binary = (mode & std::ios::binary);		// remember for later
		}

		virtual ~ifstream() {}

		// In Binary mode this function is cheap, it reads the filesize from the filesystem.
		// In Text mode this function is expensive, it reads the entire file into memory, just for the filesize.
		// There is no other way to get the filesize in text mode, because the representation on-disk is different
		// from what is read into memory. (Line endings)
		size_t file_size() {
			if (binary) {		// Binary mode
				return std::filesystem::file_size(convert(path));
			}
			else {				// Text mode: read the entire file into memory to compensate line-endings
				// Buffer
				std::string temp;													// Buffer is as large as the file on-disk
				size_t filesize_temp = std::filesystem::file_size(convert(path));   // -> Reading will make it smaller, but not larger
				temp.resize(filesize_temp);
				
				// Read file
				FS::ifstream file(path, Mode::TEXT);
				file.read(temp.data(), temp.size());
				return file.gcount();
			}
		}

		size_t read_in_chunks(size_t chunk_size, std::function<void(std::string_view)> callback) {
			std::string buffer(chunk_size, 0);
			size_t total_bytes = 0;
			
			while (!this->eof()) {
				this->read(buffer.data(), buffer.size());    // Read a chunk of the file
				size_t this_chunk_size = this->gcount();
				total_bytes += this_chunk_size;

				callback(std::string_view(buffer.data(), this_chunk_size));
			}
			
			return total_bytes;
		}

	private:
		std::wstring convert(const std::string& path) {
			return Battery::OsString(path).wstr();
		}
		
		std::string path;
		bool binary = false;
	};

	class ofstream : public std::ofstream {
	public:
		ofstream(const std::string& path, enum Mode filemode = Mode::TEXT, bool createDirectory = true)
			: std::ofstream((create_directory(createDirectory, path), convert(path)),
				(filemode == Mode::TEXT) ? (std::ios::out) : (std::ios::out | std::ios::binary)) {}

		ofstream(const std::string& path, std::ios_base::openmode mode, bool createDirectory = true)
			: std::ofstream((create_directory(createDirectory, path), convert(path)), mode) {}
		
		virtual ~ofstream() {}

	private:
		std::wstring convert(const std::string& path) {
			return Battery::OsString(path).wstr();
		}

		static void create_directory(bool create, const std::string& path) {
			if (!create)
				return;

			// File or directory already exists
			if (FS::exists(path)) {
				return;
			}
			
			// Otherwise, now create a directory
			FS::create_directories(FS::path(path).parent_path());
		}
	};

	/// <summary>
	/// Throws: 
	/// Battery::LockfileUnavailableException if the lockfile is already locked or
	/// Battery::NoSuchFileOrDirectoryException if the filename is invalid or contains directories
	/// </summary>
	class Lockfile {
	public:
		Lockfile(const std::string& filename, bool createDirectories = false);
		~Lockfile();

	private:
		void* fileDescriptor = nullptr;
	};
	

	/// <summary>
	/// Get the content of a directory. Returns an array of filenames and directory names. Must be checked with
	/// FileExists() and DirectoryExists() to know whether it's a file or directory
	/// </summary>
	/// <param name="path">- The full or relative path</param>
	/// <returns>An array with all filenames and directory names</returns>
	std::vector<std::filesystem::path> GetDirectoryContent(const std::string& path);

	/// <summary>
	/// Update the name of a file on disk. Essentially equivalent to MoveFile().
	/// The file directory can change.
	/// </summary>
	bool RenameFile(const std::string& file, const std::string& targetFile);

	/// <summary>
	/// Move a file on disk by specifying the filename (absolute or relative path)
	/// and the target directory, which is created if it does not exist.
	/// </summary>
	bool MoveFile(const std::string& file, std::string directory);

	/// <summary>
	/// Move a directory on disk. Specify the source and target directory, all files inside the 
	/// source directory are moved into the target directory recursively.
	/// Directories are created as needed. If a file fails to be moved,
	/// the process is aborted and all files are left as-is.
	/// </summary>
	bool MoveDirectory(std::string source, std::string target);

	/// <summary>
	/// Delete the content of a directory, will recursively delete all files in it. 
	/// The parent directory is not deleted. Returns true if all content was successfully deleted,
	/// but returns false when the parent did not exist
	/// </summary>
	/// <param name="path">- The directory of which the content is to remove</param>
	/// <returns>bool - if content deletion was successful</returns>
	//bool RemoveDirectoryContent(const std::string& path);

	/// <summary>
	/// Prepare an empty directory: Check if it exists, delete all content or create it, 
	/// so it's empty and ready to use
	/// </summary>
	/// <param name="path">- The directory to prepare</param>
	/// <returns>bool - if preparation was successful</returns>
	//bool PrepareEmptyDirectory(const std::string& path);

	/// <summary>
	/// Prepare a directory: Check if it exists or create it, but it does not delete any content if
	/// it already exists
	/// </summary>
	/// <param name="path">- The directory to prepare</param>
	/// <returns>bool - if preparation was successful</returns>
	//bool PrepareDirectory(const std::string& path);






	/// <summary>
	/// Get the full path of the currently running executable, for example "C:/some/path/main.exe"
	/// </summary>
	/// <exception cref="Battery::Exception - Thrown when Allegro was not initialized before this function call"></exception>
	/// <returns>std::string - the full path or "" if invalid</returns>
	std::string GetExecutablePath();

	/// <summary>
	/// Get the name of the executable, for example "main.exe"
	/// </summary>
	/// <exception cref="Battery::Exception - Thrown when Allegro was not initialized before this function call"></exception>
	/// <returns>std::string - the name or "" if invalid</returns>
	std::string GetExecutableName();

	/// <summary>
	/// Get the directory in which the executable is placed, for example "C:/some/path/" when
	/// the executable was "C:/some/path/main.exe"
	/// </summary>
	/// <exception cref="Battery::Exception - Thrown when Allegro was not initialized before this function call"></exception>
	/// <returns>std::string - the full path or "" if invalid</returns>
	std::string GetExecutableDirectory();







	/// <summary>
	/// Get the filename from a full path (file or directory), for example: Pass "C:/some/file.txt" and get "file.txt".
	/// Trailing forward '/'s are removed and treated as a file.
	/// </summary>
	/// <param name="path">- The full or relative path to process</param>
	/// <returns>std::string - Only the last part of the path</returns>
	std::string GetFilename(const std::string& path);

	/// <summary>
	/// Get the basename from a full path, for example: Pass "C:/some/file.txt" and get "file".
	/// Returns the full name of a directory.
	/// </summary>
	/// <param name="path">- The full or relative path to process</param>
	/// <returns>std::string - The basename</returns>
	std::string GetBasename(const std::string& path);

	/// <summary>
	/// Get the extension from a full path filename, for example: Pass "C:/some/multiple.dots.txt" and get ".txt".
	/// Returns none ("") if last filename does not contain a dot.
	/// </summary>
	/// <param name="path">- The full or relative path to process</param>
	/// <returns>std::string - The extension</returns>
	std::string GetExtension(const std::string& path);

	/// <summary>
	/// Get an array with every component of a path, for example: Pass "C:/some/file.txt" and get {"C:", "some", "file.txt"}
	/// or pass "../directory" and get {"..", "directory"}
	/// </summary>
	/// <param name="path">- The full or relative path to process</param>
	/// <returns>std::vector&lt;std::string&gt; - An array with every component</returns>
	std::vector<std::string> GetPathComponents(const std::string& path);

	/// <summary>
	/// Get the directory in which the target is lying, for example: "C:/some/file.txt" -> "C:/some/".
	/// Also works for folders.
	/// </summary>
	/// <param name="path">- The full or relative path to process</param>
	/// <returns>std::string - Basically the path without the filename</returns>
	std::string GetParentDirectory(const std::string& path);

	/// <summary>
	/// Get an Allegro standard path from al_get_standard_path(). Available paths are ALLEGRO_RESOURCES_PATH, ALLEGRO_TEMP_PATH, 
	/// ALLEGRO_USER_HOME_PATH, ALLEGRO_USER_DOCUMENTS_PATH, ALLEGRO_USER_DATA_PATH, ALLEGRO_USER_SETTINGS_PATH, ALLEGRO_EXENAME_PATH
	/// </summary>
	/// <param name="id">- An Allegro enum from above</param>
	/// <exception cref="Battery::Exception - Thrown when the given Allegro enum is invalid"></exception>
	/// <returns>std::string - The path returned from Allegro</returns>
	std::string GetAllegroStandardPath(int id);










	/// <summary>
	/// Execute shell commands (basic system() call)
	/// </summary>
	/// <param name="command"></param>
	/// <returns></returns>
	std::pair<bool, size_t> ExecuteShellCommand(const std::string& command);

	/// <summary>
	/// Execute shell commands in a separate process. Returns a pair of bool and a return code.
	/// If the command runs successfully, returns true and the exit code of the command.
	/// Otherwise, returns false and the error code of GetLastError() of ShellExecuteEx(). 
	/// Do not use this function to call processes that create windows.
	/// </summary>
	/// <param name="command"></param>
	/// <returns></returns>
	std::pair<bool, size_t> ExecuteShellCommandSilent(const std::string& command, bool hidden = true);







	/// <summary>
	/// Unzip a .zip file to a target directory using powershell Expand-Archive call.
	/// Existing files are overwritten by default. The target directory will be created as needed.
	/// </summary>
	bool ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force = true);

	/// <summary>
	/// Load an embedded file from the resource compiler (.rc file). Parameter is the Win32 ID, 
	/// taken from 'resource.h'. Second parameter is the expected type from resource.rc ("ICON", "PNG", "FONT", ...)
	/// </summary>
	std::vector<uint8_t> LoadEmbeddedResource(int id, const char* type);

}
