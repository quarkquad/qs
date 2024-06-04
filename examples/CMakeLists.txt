if(APPLE)
    set(PLUGIN_VARIANTS AU VST3)
else()
    set(PLUGIN_VARIANTS VST3)
endif()

find_package(JUCE CONFIG REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(QSEncoder)
add_subdirectory(QSDecoder)
