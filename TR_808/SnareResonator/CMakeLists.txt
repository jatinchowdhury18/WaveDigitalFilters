create_wdf_plugin(SnareResonator SnrR)

target_link_libraries(SnareResonator
    PRIVATE
        chowdsp::chowdsp_simd
)
