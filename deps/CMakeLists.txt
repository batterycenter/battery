
# reproc
set(REPROC++ ON CACHE BOOL "" FORCE)
add_subdirectory(reproc)
add_library(reproc::reproc++ ALIAS reproc++)

# spdlog
add_subdirectory(spdlog)

# utfcpp
add_subdirectory(utfcpp)