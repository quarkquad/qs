set(TEST_PROJECT_NAME qs_test)

find_package(JUCE CONFIG REQUIRED)
find_package(Catch2 3 REQUIRED)
include(Catch)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

juce_add_console_app(${TEST_PROJECT_NAME}
    PRODUCT_NAME ${TEST_PROJECT_NAME})

set_property(TARGET ${TEST_PROJECT_NAME} PROPERTY CXX_STANDARD 14)

target_sources(${TEST_PROJECT_NAME} PRIVATE test.cpp)

target_link_libraries(${TEST_PROJECT_NAME} 
    PRIVATE
    quark::qs
    juce::juce_dsp
    Catch2::Catch2WithMain
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

catch_discover_tests(${TEST_PROJECT_NAME})
