create_wdf_plugin(BBDDelay BBdd)

target_link_libraries(BBDDelay PRIVATE
    PRIVATE
        chowdsp::chowdsp_sources
        chowdsp::chowdsp_filters
)
