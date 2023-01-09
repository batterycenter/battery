#pragma once

#include "battery/core/environment.h"
#include "battery/core/OsString.h"
#include <string>

namespace battery {

    // TODO: Allow to retrieve the error message in a specific language
#ifdef BATTERY_ARCH_WINDOWS
    std::string get_last_win32_error();       // This function is only available on windows, for writing the
                                              // windows-specific side of cross-platform functions

    enum class MB_Buttons {
        OK,
        OK_CANCEL,
        RETRY_CANCEL,
        YES_NO,
        YES_NO_CANCEL,
        HELP,
        CANCEL_TRY_CONTINUE,
        ABORT_RETRY_IGNORE
    };

    enum class MB_Status {
        OK,
        YES,
        NO,
        CANCEL,			// also contains ABORT
        RETRY,			// also contains TRY_AGAIN
        CONTINUE		// also contains IGNORE
    };

    // defaultButton: 1, 2, 3 or 4
    MB_Status message_box_info(const OsString& message, const OsString& title = "Information", MB_Buttons buttons = MB_Buttons::OK, int defaultButton = 1);
    MB_Status message_box_warning(const OsString& message, const OsString& title = "Warning", MB_Buttons buttons = MB_Buttons::OK, int defaultButton = 1);
    MB_Status message_box_error(const OsString& message, const OsString& title = "Error", MB_Buttons buttons = MB_Buttons::OK, int defaultButton = 1);
#endif

}
