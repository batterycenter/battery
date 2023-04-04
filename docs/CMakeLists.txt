
find_package(Doxygen)
if (NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen executable not found. Either install it (Hint: 'sudo apt install doxygen'), or use -DBUILD_DOCS=false to build without documentation.")
endif ()

battery_add_package("gh:jothepro/doxygen-awesome-css#v2.2.0")   # Modern Doxygen CSS theme

# Before generating doxygen, build the battery cli and use it to generate the Doxyfile

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_LIST_DIR}/build/Doxyfile
    COMMAND ${CMAKE_BINARY_DIR}/bin/b generate ${CMAKE_CURRENT_LIST_DIR}/Doxyfile_template ${CMAKE_CURRENT_LIST_DIR}/build/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..
    COMMENT "Generating Doxyfile"
    DEPENDS battery::cli
    VERBATIM)

# Define a target that builds the documentation using Doxygen.
# However, it is not part of BUILD_ALL (you must specify the docs target manually).

add_custom_target(docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/build/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..
    COMMENT "Generating API documentation with Doxygen"
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/build/Doxyfile
    VERBATIM)
