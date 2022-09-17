create_wdf_plugin(HatResonator HatR)

target_link_libraries(HatResonator
    PRIVATE
        chowdsp::chowdsp_simd
)
