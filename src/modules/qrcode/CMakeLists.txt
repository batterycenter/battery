
# Usage of this module:
# To be able to convert a QR Code into an sf::Image, you need to add the 'graphics' module first!!!
#
# battery_add_module(qrcode)
# target_link_libraries(... PUBLIC battery::qrcode)

option(BATTERY_QRCODE_SILENT "If true, the 'qrcode' module will not warn you if the 'graphics' module was not loaded before it." OFF)

b_add_library(battery_qrcode STATIC ALIAS battery::qrcode)
b_add_source_files(battery_qrcode   # This function also groups the files in the IDE
        src/qrcode.cpp
    )

target_link_libraries(battery_qrcode PUBLIC battery::core)

if (${BATTERY_MODULE_LOADED_graphics})
    target_compile_definitions(battery_qrcode PUBLIC BATTERY_QRCODE_HAVE_GRAPHICS)
    target_link_libraries(battery_qrcode PUBLIC battery::graphics)
else()
    if (NOT BATTERY_QRCODE_SILENT)
        message(WARNING "The 'graphics' module was not loaded before the 'qrcode' module, some qrcode features will be disabled! Adjust your module imports or set BATTERY_QRCODE_SILENT to true to disable this warning.")
    endif()
endif()