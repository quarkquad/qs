project(QSDecoder VERSION 1.0)

juce_add_plugin(${PROJECT_NAME}
    VERSION "1.0"
    BUNDLE_ID "com.dublab.QSEncoder"
    COMPANY_WEBSITE "https://dublab.com"
    COMPANY_EMAIL "info@dublab.com"
    COMPANY_NAME "dublab"
    HARDENED_RUNTIME_ENABLED YES
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Dubl
    PLUGIN_CODE Qsed
    FORMATS "${PLUGIN_VARIANTS}"
    AU_MAIN_TYPE "kAudioUnitType_Effect"
    VST3_CATEGORIES "Fx"
    DESCRIPTION "QSDecoder"
    PRODUCT_NAME "QSDecoder"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

target_sources(${PROJECT_NAME}
    PRIVATE 
    PluginEditor.cpp
    PluginProcessor.cpp)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    quark::qs
    juce::juce_dsp
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)
