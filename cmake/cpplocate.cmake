
# set(CPPLOCATE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/cpplocate)
# add_library(cpplocate STATIC)
# add_library(cpplocate::cpplocate ALIAS cpplocate)

# target_sources(cpplocate PRIVATE
#         ${CPPLOCATE_DIR}/source/cpplocate/source/cpplocate.cpp
# )

# target_include_directories(cpplocate PUBLIC
#     $<BUILD_INTERFACE:${CPPLOCATE_DIR}/source/cpplocate/include>
#     $<INSTALL_INTERFACE:/>
# )

# install(
#     TARGETS cpplocate
#     EXPORT cpplocateTargets
#     LIBRARY DESTINATION "lib"
#     ARCHIVE DESTINATION "lib"
#     RUNTIME DESTINATION "bin"
#     INCLUDES DESTINATION "include"
# )

# install(
#     EXPORT cpplocateTargets 
#     DESTINATION "lib/cmake/cpplocate"
#     NAMESPACE cpplocate::
# )

# # Install headers
# install(
#     DIRECTORY "${CPPLOCATE_DIR}/"
#     DESTINATION "include"
#     FILES_MATCHING PATTERN "*.h"
# )

# install(
#     FILES "cmake/cpplocateConfig.cmake"
#     DESTINATION "lib/cmake/cpplocate"
# )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/cpplocate)
