
#include "battery/core/environment.hpp"
#include "battery/core/messages.hpp"
#include "battery/core/string.hpp"
#include "battery/core/process.hpp"

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

        auto buffer = b::from_osstring(messageBuffer);
        LocalFree(messageBuffer);
        return buffer;
    }
    static void win32_message_box(const std::string& message, UINT options) {
        auto text = b::to_osstring(message);
        ::MessageBoxW(nullptr, std::bit_cast<const wchar_t*>(text.c_str()), L"Error", options);
    }
#else
    static void linux_run_zenity(const std::vector<std::string>& commands) {
        battery::process zenity;
        zenity.options.executable = "zenity";
        zenity.options.arguments = commands;
        zenity.execute_sync();
    }
#endif  // BATTERY_ARCH_WINDOWS

    void message_box_info(const std::string& message) {
#ifdef BATTERY_ARCH_WINDOWS
        win32_message_box(message, MB_ICONINFORMATION | MB_OK);
#else
        linux_run_zenity({ "--info", "--text=" + message });
#endif
    }

    void message_box_warning(const std::string& message) {
#ifdef BATTERY_ARCH_WINDOWS
        win32_message_box(message, MB_ICONWARNING | MB_OK);
#else
        linux_run_zenity({ "--warning", "--text=" + message });
#endif
    }

    void message_box_error(const std::string& message) {
#ifdef BATTERY_ARCH_WINDOWS
        win32_message_box(message, MB_ICONERROR | MB_OK);
#else
        linux_run_zenity({ "--error", "--text=" + message });
#endif
    }

}
