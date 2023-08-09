
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
            DWORD const errorMessageID = ::GetLastError();
            if (errorMessageID == 0) {
                return ""_b;
            }

            LPWSTR messageBuffer = nullptr;
            FormatMessageW(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    nullptr,
                    errorMessageID,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    std::bit_cast<LPWSTR>(& messageBuffer),
                    0,
                    nullptr);

            auto tmp = b::string::decode_native(std::wstring(messageBuffer));
            LocalFree(messageBuffer);
            return tmp;
        }
        static void win32_message_box(const b::string& message, const b::string& title, UINT options) {
            ::MessageBoxW(nullptr, message.encode_native().c_str(), title.encode_native().c_str(), options);
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
        internal::win32_message_box(message, "Info"_b, MB_ICONINFORMATION | MB_OK);
#else
        internal::linux_run_zenity({ "--info", "--text=" + message });
#endif
    }

    void message_box_warning(const b::string& message) {
#ifdef B_OS_WINDOWS
        internal::win32_message_box(message, "Warning"_b, MB_ICONWARNING | MB_OK);
#else
        internal::linux_run_zenity({ "--warning", "--text=" + message });
#endif
    }

    void message_box_error(const b::string& message) {
#ifdef B_OS_WINDOWS
        internal::win32_message_box(message, "Error"_b, MB_ICONERROR | MB_OK);
#else
        internal::linux_run_zenity({ "--error", "--text=" + message });
#endif
    }

}
