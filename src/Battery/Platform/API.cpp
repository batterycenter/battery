
#include "Battery/pch.h"
#include "Battery/Platform/API.h"

#ifdef _WIN32
#include "windows.h"

namespace Battery {
	namespace Platform {

		std::string GetLastErrorAsString() {

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
}
#endif