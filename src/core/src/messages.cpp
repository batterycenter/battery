
#include "battery/core/messages.hpp"
#include "battery/core/string.hpp"
#include "battery/core/process.hpp"

#ifdef B_OS_WINDOWS
#include <Windows.h>
#endif

// TODO: Check if zenity is available at program startup

namespace b {

    namespace internal {

#ifdef B_OS_WINDOWS
        b::string get_last_win32_error() {
            DWORD errorMessageID = ::GetLastError();
            if (errorMessageID == 0)
                return "";

            LPWSTR messageBuffer = nullptr;
            FormatMessageW(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) & messageBuffer, 0,
                    nullptr);

            auto buffer = b::string(std::wstring(messageBuffer));
            LocalFree(messageBuffer);
            return buffer;
        }
        static void win32_message_box(const b::string& message, UINT options) {
            ::MessageBoxW(nullptr, std::bit_cast<const wchar_t*>(message.c_str()), L"Error", options);
        }
#else
        void linux_run_zenity(const std::vector<b::string>& commands) {
            b::process zenity;
            zenity.options.executable = "zenity";
            zenity.options.arguments = commands;
            zenity.execute_sync();
        }
#endif
    }  // namespace internal

    void message_box_info(const b::string& message) {
#ifdef B_OS_WINDOWS
        internal::win32_message_box(message, MB_ICONINFORMATION | MB_OK);
#else
        internal::linux_run_zenity({ "--info", "--text=" + message });
#endif
    }

    void message_box_warning(const b::string& message) {
#ifdef B_OS_WINDOWS
        internal::win32_message_box(message, MB_ICONWARNING | MB_OK);
#else
        internal::linux_run_zenity({ "--warning", "--text=" + message });
#endif
    }

    void message_box_error(const b::string& message) {
#ifdef B_OS_WINDOWS
        internal::win32_message_box(message, MB_ICONERROR | MB_OK);
#else
        internal::linux_run_zenity({ "--error", "--text=" + message });
#endif
    }

}
