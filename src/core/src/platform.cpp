
#include "battery/core/environment.h"
#include "battery/core/platform.h"
#include "battery/core/OsString.h"

#ifdef BATTERY_ARCH_WINDOWS
#include <windows.h>
#endif

namespace battery {

#ifdef BATTERY_ARCH_WINDOWS
    std::string get_last_win32_error() {

        DWORD errorMessageID = ::GetLastError();
        if (errorMessageID == 0)
            return "";

        LPWSTR messageBuffer = nullptr;
        size_t size = FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) & messageBuffer, 0,
                nullptr);

        OsString message(std::wstring(messageBuffer, size));
        LocalFree(messageBuffer);

        return message;
    }



    static MB_Status win32_to_enum(int code) {
        if (code == IDOK) return MB_Status::OK;
        if (code == IDYES) return MB_Status::YES;
        if (code == IDNO) return MB_Status::NO;
        if (code == IDCANCEL) return MB_Status::CANCEL;
        if (code == IDABORT) return MB_Status::CANCEL;
        if (code == IDRETRY) return MB_Status::RETRY;
        if (code == IDTRYAGAIN) return MB_Status::RETRY;
        if (code == IDCONTINUE) return MB_Status::CONTINUE;
        if (code == IDIGNORE) return MB_Status::CONTINUE;
        return MB_Status::OK;
    }

    static int enum_to_win32_code(MB_Buttons buttons) {
        if (buttons == MB_Buttons::OK) return MB_OK;
        if (buttons == MB_Buttons::OK_CANCEL) return MB_OKCANCEL;
        if (buttons == MB_Buttons::RETRY_CANCEL) return MB_RETRYCANCEL;
        if (buttons == MB_Buttons::YES_NO) return MB_YESNO;
        if (buttons == MB_Buttons::YES_NO_CANCEL) return MB_YESNOCANCEL;
        if (buttons == MB_Buttons::HELP) return MB_HELP;
        if (buttons == MB_Buttons::CANCEL_TRY_CONTINUE) return MB_CANCELTRYCONTINUE;
        if (buttons == MB_Buttons::ABORT_RETRY_IGNORE) return MB_ABORTRETRYIGNORE;
        return MB_OK;
    }

    MB_Status message_box_info(const OsString& message, const OsString& title, MB_Buttons buttons, int defaultButton) {
        int code = ::MessageBoxW(nullptr, message.wstr().c_str(), title.wstr().c_str(),
                                 MB_ICONINFORMATION | enum_to_win32_code(buttons) | ((defaultButton - 1) * 256));
        return win32_to_enum(code);
    }

    MB_Status message_box_warning(const OsString& message, const OsString& title, MB_Buttons buttons, int defaultButton) {
        int code = ::MessageBoxW(nullptr, message.wstr().c_str(), title.wstr().c_str(),
                                 MB_ICONWARNING | enum_to_win32_code(buttons) | ((defaultButton - 1) * 256));
        return win32_to_enum(code);
    }

    MB_Status message_box_error(const OsString& message, const OsString& title, MB_Buttons buttons, int defaultButton) {
        int code = ::MessageBoxW(nullptr, message.wstr().c_str(), title.wstr().c_str(),
                                 MB_ICONERROR | enum_to_win32_code(buttons) | ((defaultButton - 1) * 256));
        return win32_to_enum(code);
    }
#endif

}
