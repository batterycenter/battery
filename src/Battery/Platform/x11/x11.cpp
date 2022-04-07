
#include "Battery/Platform/Platform.h"
#include "Battery/StringUtils.h"

#include <allegro5/allegro_x.h>

#include <unistd.h>
#include <sys/file.h>

namespace Battery {

	void* platform_LockFileDescriptor(const std::string& file) {
		int fileDescriptor = open(file.c_str(), O_CREAT | O_RDWR, 0666);
		if (fileDescriptor <= 0) {
			throw Battery::NoSuchFileOrDirectoryException();
		}
		if (flock(fileDescriptor, LOCK_EX | LOCK_NB) != 0) {
			platform_UnlockFileDescriptor((void*)(size_t)fileDescriptor);
			throw Battery::LockfileUnavailableException();
		}
		return (void*)(size_t)fileDescriptor;
	}

	void platform_UnlockFileDescriptor(void* fileDescriptor) {
		flock((size_t)fileDescriptor, LOCK_UN);
		close((size_t)fileDescriptor);
	}

	bool platform_ExtractArchive(const std::string& file, const std::string& targetDirectory, bool force) {
#warning TODO: Implement unzipping
		return false;
	}

    std::pair<bool, size_t> platform_ExecuteShellCommandSilent(const std::string& command, bool hidden) {
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}
	
	ALLEGRO_FILE* platform_LoadEmbeddedResource(int id, const char* type) {
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}

	HWND platform_GetWinHandle(ALLEGRO_DISPLAY* allegroDisplayPointer) {
		throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
	}

	XID platform_GetWindowIdentifier(ALLEGRO_DISPLAY* allegroDisplayPointer) {
		return al_get_x_window_id(allegroDisplayPointer);
	}

	bool platform_IsFocused(ALLEGRO_DISPLAY* allegroDisplayPointer) {

		Display* dpy = XOpenDisplay(NULL);
  		if (dpy == NULL)  {
			return false;
  		}

		XID focused;
		int revert_to;
		XGetInputFocus(dpy, &focused, &revert_to);
  		XCloseDisplay(dpy);

		return focused == platform_GetWindowIdentifier(allegroDisplayPointer);
	}

	bool platform_Focus(ALLEGRO_DISPLAY* allegroDisplayPointer) {

		Display* dpy = XOpenDisplay(NULL);
  		if (dpy == NULL)  {
			return false;
  		}

		XSetInputFocus(dpy, platform_GetWindowIdentifier(allegroDisplayPointer), RevertToNone, CurrentTime);
		XRaiseWindow(dpy, platform_GetWindowIdentifier(allegroDisplayPointer));
  		XCloseDisplay(dpy);

		return true;
	}

	bool platform_Hide(ALLEGRO_DISPLAY* allegroDisplayPointer) {
		//throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
		LOG_WARN("TODO: Implement platform_Hide");
		return false;
	}

	bool platform_Show(ALLEGRO_DISPLAY* allegroDisplayPointer) {
		//throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
		LOG_WARN("TODO: Implement platform_Show");
		return false;
	}

	void platform_HideFromTaskbar(ALLEGRO_DISPLAY* allegroDisplayPointer) {
		//throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
		LOG_WARN("TODO: Implement platform_HideFromTaskbar");
	}

	void platform_ShowInTaskbar(ALLEGRO_DISPLAY* allegroDisplayPointer) {
		//throw Battery::NotImplementedException(__PRETTY_FUNCTION__);
		LOG_WARN("TODO: Implement platform_ShowInTaskbar");
	}
}
