
find_package(Doxygen)
if (NOT DOXYGEN_FOUND)
    if (WIN32)
        set(HINT "")
    else ()
        set(HINT " (Hint: 'sudo apt install doxygen')")
    endif ()
    message(FATAL_ERROR "Doxygen executable not found. Either install it${HINT}, or use -DBUILD_DOCS=false to build without documentation.")
endif ()

configure_file(${CMAKE_CURRENT_LIST_DIR}/Doxyfile.in ${CMAKE_CURRENT_LIST_DIR}/build/Doxyfile)

# Define a target that builds the documentation using Doxygen.
# However, it is not part of BUILD_ALL (you must specify the docs target manually).

add_custom_target(docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/build/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..
    COMMENT "Generating API documentation with Doxygen"
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/build/Doxyfile
    VERBATIM)
