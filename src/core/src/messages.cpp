
#include "battery/core/environment.h"
#include "battery/core/messages.h"
#include "battery/core/string.h"
#include "battery/core/process.h"

#ifdef BATTERY_ARCH_WINDOWS
#include <windows.h>
#endif

// TODO: Check if zenity is available at program startup

namespace battery {

#ifdef BATTERY_ARCH_WINDOWS
    std::string get_last_win32_error() {

        DWORD errorMessageID = ::GetLastError();
        if (errorMessageID == 0)
            return "";

        LPWSTR messageBuffer = nullptr;
        FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) & messageBuffer, 0,
                nullptr);

        auto buffer = battery::string::osstring_to_utf8(messageBuffer);
        LocalFree(messageBuffer);
        return buffer;
    }
#endif  // BATTERY_ARCH_WINDOWS

    void message_box_info(const std::string& message) {
#ifdef BATTERY_ARCH_WINDOWS
        auto text = battery::string::utf8_to_osstring(message);
        ::MessageBoxW(nullptr, std::bit_cast<const wchar_t*>(text.c_str()), L"Error", MB_ICONINFORMATION | MB_OK);
#else
        battery::process zenity;
        zenity.options.executable = "zenity";
        zenity.options.arguments = { "--info", "--text=" + message };
        zenity.execute_sync();
#endif
    }

    void message_box_warning(const std::string& message) {
#ifdef BATTERY_ARCH_WINDOWS
        auto text = battery::string::utf8_to_osstring(message);
        ::MessageBoxW(nullptr, std::bit_cast<const wchar_t*>(text.c_str()), L"Error", MB_ICONWARNING | MB_OK);
#else
        battery::process zenity;
        zenity.options.executable = "zenity";
        zenity.options.arguments = { "--warning", "--text=" + message };
        zenity.execute_sync();
#endif
    }

    void message_box_error(const std::string& message) {
#ifdef BATTERY_ARCH_WINDOWS
        auto text = battery::string::utf8_to_osstring(message);
        ::MessageBoxW(nullptr, std::bit_cast<const wchar_t*>(text.c_str()), L"Error", MB_ICONERROR | MB_OK);
#else
        battery::process zenity;
        zenity.options.executable = "zenity";
        zenity.options.arguments = { "--error", "--text=" + message };
        zenity.execute_sync();
#endif
    }

}
