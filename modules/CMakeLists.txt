add_subdirectory(JUCE)
include(cmake/SubprojectVersion.cmake)
subproject_version(JUCE juce_version)
message(STATUS "VERSION for JUCE: ${juce_version}")

# add custom JUCE modules
add_subdirectory(chowdsp_utils)

include(cmake/WarningFlags.cmake)
add_library(juce_plugin_modules STATIC)
target_link_libraries(juce_plugin_modules
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_plugin_client
        juce::juce_dsp
        chowdsp_dsp
        chowdsp_plugin_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        warning_flags
)

target_compile_definitions(juce_plugin_modules
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_JACK=1
        JUCE_ALSA=1
        JucePlugin_Manufacturer="chowdsp"
        JucePlugin_VersionString="${CMAKE_PROJECT_VERSION}"
        JucePlugin_Name="${CMAKE_PROJECT_NAME}"
        JUCE_MODAL_LOOPS_PERMITTED=0
    INTERFACE
        $<TARGET_PROPERTY:juce_plugin_modules,COMPILE_DEFINITIONS>
)

target_include_directories(juce_plugin_modules
    INTERFACE
        $<TARGET_PROPERTY:juce_plugin_modules,INCLUDE_DIRECTORIES>
)

set_target_properties(juce_plugin_modules PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE
    VISIBILITY_INLINES_HIDDEN TRUE
    C_VISBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
)

include(cmake/CreateWDFPlugin.cmake)
