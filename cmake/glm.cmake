
set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/glm)
add_library(glm INTERFACE)
add_library(glm::glm ALIAS glm)

target_include_directories(glm INTERFACE
    $<BUILD_INTERFACE:${GLM_DIR}/>
    $<INSTALL_INTERFACE:/>
)

install(
    TARGETS glm
    EXPORT glmTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)

install(
    EXPORT glmTargets 
    DESTINATION "lib/cmake/glm"
    NAMESPACE glm::
)

# Install headers
install(
    DIRECTORY "${GLM_DIR}/"
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl"
)

install(
    FILES "cmake/glmConfig.cmake"
    DESTINATION "lib/cmake/glm"
)
