cmake_minimum_required(VERSION 3.26)

project(QS VERSION 0.1.0 LANGUAGES CXX)
include(CMakePackageConfigHelpers)

set(QS_NAMESPACE quark)

option(QS_TESTS "Enable testing" OFF)
if (QS_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()

option(QS_EXAMPLES "Enable sample plugins" OFF)
if (QS_EXAMPLES)
    add_subdirectory(examples)
endif()

if(COMMAND juce_add_module)
    juce_add_module(qs ALIAS_NAMESPACE "${QS_NAMESPACE}")
endif()

set(QS_INCLUDE_PATH "include/${PROJECT_NAME}-${QS_VERSION}")
install(
    DIRECTORY qs 
    DESTINATION ${QS_INCLUDE_PATH}
    PATTERN ".DS_Store" EXCLUDE
)

# QSConfigVersion.cmake
set(QS_CMAKE_CONFIG_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
write_basic_package_version_file(
    ${QS_CMAKE_CONFIG_VERSION_FILE}
    COMPATIBILITY SameMajorVersion
)

# QSConfig.cmake
set(QS_CMAKE_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake")
set(QS_PACKAGE_INSTALL_PATH "lib/cmake/${PROJECT_NAME}-${QS_VERSION}")
configure_package_config_file(
    "${PROJECT_NAME}Config.cmake.in"
    ${QS_CMAKE_CONFIG_FILE}
    INSTALL_DESTINATION ${QS_PACKAGE_INSTALL_PATH}
    PATH_VARS QS_INCLUDE_PATH
)

install(
    FILES ${QS_CMAKE_CONFIG_FILE} ${QS_CMAKE_CONFIG_VERSION_FILE} 
    DESTINATION ${QS_PACKAGE_INSTALL_PATH}
)
