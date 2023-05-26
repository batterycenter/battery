
#include "battery/core/environment.hpp"
#include "battery/core/messages.hpp"
#include "battery/core/string.hpp"
#include "battery/core/process.hpp"

#ifdef BATTERY_ARCH_WINDOWS
#include <windows.h>
#endif

// TODO: Check if zenity is available at program startup

namespace b {

    namespace internal {

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

        auto buffer = b::osstring_to_u8(messageBuffer);
        LocalFree(messageBuffer);
        return b::u8_as_str(buffer);
    }
    static void win32_message_box(const std::u8string& message, UINT options) {
        auto text = b::to_osstring(message);
        ::MessageBoxW(nullptr, std::bit_cast<const wchar_t*>(text.c_str()), L"Error", options);
    }
#else
    static void linux_run_zenity(const std::vector<std::string>& commands) {
        b::process zenity;
        zenity.options.executable = "zenity";
        zenity.options.arguments = commands;
        zenity.execute_sync();
    }
#endif  // BATTERY_ARCH_WINDOWS

    }  // namespace internal

    void message_box_info(const std::string& message) {
        return message_box_info(b::u8_from_std_string(message));
    }

    void message_box_info(const std::u8string& message) {
#ifdef BATTERY_ARCH_WINDOWS
        internal::win32_message_box(message, MB_ICONINFORMATION | MB_OK);
#else
        internal::linux_run_zenity({ "--info", "--text=" + b::u8_as_str(message) });
#endif
    }

    void message_box_info(const std::u32string& message) {
        return message_box_info(b::to_u8(message));
    }




    void message_box_warning(const std::string& message) {
        return message_box_warning(b::u8_from_std_string(message));
    }

    void message_box_warning(const std::u8string& message) {
#ifdef BATTERY_ARCH_WINDOWS
        internal::win32_message_box(message, MB_ICONWARNING | MB_OK);
#else
        internal::linux_run_zenity({ "--warning", "--text=" + b::u8_as_str(message) });
#endif
    }

    void message_box_warning(const std::u32string& message) {
        return message_box_warning(b::to_u8(message));
    }





    void message_box_error(const std::string& message) {
        return message_box_error(b::u8_from_std_string(message));
    }

    void message_box_error(const std::u8string& message) {
#ifdef BATTERY_ARCH_WINDOWS
        internal::win32_message_box(message, MB_ICONERROR | MB_OK);
#else
        internal::linux_run_zenity({ "--error", "--text=" + b::u8_as_str(message) });
#endif
    }

    void message_box_error(const std::u32string& message) {
        return message_box_error(b::to_u8(message));
    }

}
