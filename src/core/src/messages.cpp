
#include "battery/messages.hpp"
#include "battery/string.hpp"
#include "battery/process.hpp"

#ifdef B_OS_WINDOWS
#include <Windows.h>
#endif

#ifdef B_OS_WEB
#include <emscripten.h>
extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void emscripten_alert_message_box(const char* title, const char* message) {
    EM_ASM({
      var title = UTF8ToString($0);
      var message = UTF8ToString($1);
      alert(message);
    }, title, message);
  }
}
#endif // B_OS_WEB

// TODO: Check if zenity is available at program startup

namespace b {

    namespace internal {

#ifdef B_OS_WINDOWS
        std::string get_last_win32_error() {
            DWORD const errorMessageID = ::GetLastError();
            if (errorMessageID == 0) {
                return "";
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

            auto tmp = b::narrow(std::wstring(messageBuffer));
            LocalFree(messageBuffer);
            return tmp;
        }
        static void win32_message_box(const std::string& message, const std::string& title, UINT options) {
            ::MessageBoxW(nullptr,
                          b::widen(message).c_str(),
                          b::widen(title).c_str(),
                          options);
        }
#elif defined(B_OS_WEB)
        void emscripten_alert(const std::string& message, const std::string& title) {
            emscripten_alert_message_box(title.c_str(), message.c_str());
        }
#else
        void linux_run_zenity(const std::vector<std::string>& commands) {
            b::process zenity;
            zenity.options.executable = "zenity";
            zenity.options.arguments = commands;
            zenity.execute_sync();
        }
#endif
    }  // namespace internal

    void message_box_info(const std::string& message) {
#ifdef B_OS_WINDOWS
        internal::win32_message_box(message, "Info", MB_ICONINFORMATION | MB_OK);
#elif defined(B_OS_WEB)
        internal::emscripten_alert(message, "Info");
#else
        internal::linux_run_zenity({ "--info", "--text=" + message });
#endif
    }

    void message_box_warning(const std::string& message) {
#ifdef B_OS_WINDOWS
        internal::win32_message_box(message, "Warning", MB_ICONWARNING | MB_OK);
#elif defined(B_OS_WEB)
        internal::emscripten_alert(message, "Warning");
#else
        internal::linux_run_zenity({ "--warning", "--text=" + message });
#endif
    }

    void message_box_error(const std::string& message) {
#ifdef B_OS_WINDOWS
        internal::win32_message_box(message, "Error", MB_ICONERROR | MB_OK);
#elif defined(B_OS_WEB)
        internal::emscripten_alert(message, "Error");
#else
        internal::linux_run_zenity({ "--error", "--text=" + message });
#endif
    }

}
