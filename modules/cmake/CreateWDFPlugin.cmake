# create_wdf_plugin(<plugin-name> <plugin-code>)
#
# Creates an audio plugin for a WDF with this root directory
function(create_wdf_plugin plugin_name plugin_code)
    message(STATUS "Setting up WDF plugin: ${plugin_name}")

    set(JUCE_FORMATS AU VST3 Standalone)
    juce_add_plugin(${plugin_name}
            COMPANY_NAME chowdsp
            PLUGIN_MANUFACTURER_CODE Chow
            PLUGIN_CODE ${plugin_code}
            FORMATS ${JUCE_FORMATS}

            MICROPHONE_PERMISSION_ENABLED TRUE
            )

    juce_generate_juce_header(${plugin_name})
    target_compile_definitions(${plugin_name} PUBLIC JUCE_VST3_CAN_REPLACE_VST2=0)
    target_link_libraries(${plugin_name} PRIVATE juce_plugin_modules)

    target_sources(${plugin_name} PRIVATE src/PluginProcessor.cpp)
endfunction()
