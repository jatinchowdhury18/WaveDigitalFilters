# create_wdf_plugin(<plugin-name> <plugin-code>)
#
# Creates an audio plugin for a WDF with this root directory
function(create_wdf_plugin plugin_name plugin_code)
    message(STATUS "Setting up WDF plugin: ${plugin_name}")

    set(JUCE_FORMATS AU VST3 Standalone)
    juce_add_plugin(${plugin_name}
        COMPANY_NAME "WDF Plugins"
        PLUGIN_MANUFACTURER_CODE WDFs
        PLUGIN_CODE ${plugin_code}
        FORMATS ${JUCE_FORMATS}

        MICROPHONE_PERMISSION_ENABLED TRUE
    )

    target_compile_definitions(${plugin_name} PUBLIC
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_JACK=1
        JUCE_ALSA=1
        JUCE_MODAL_LOOPS_PERMITTED=0
    )

    target_link_libraries(${plugin_name} PRIVATE
        PRIVATE
            juce::juce_dsp
            chowdsp_dsp
            chowdsp_plugin_utils
            chowdsp_wdf
        PUBLIC
            juce::juce_recommended_config_flags
            juce::juce_recommended_lto_flags
            warning_flags
    )

    target_sources(${plugin_name} PRIVATE src/PluginProcessor.cpp)

endfunction()
