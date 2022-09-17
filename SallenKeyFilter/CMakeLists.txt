create_wdf_plugin(SallenKeyFilter Skft)

target_sources(SallenKeyFilter PRIVATE
    src/SallenKeyLPFWDF.cpp
)

target_link_libraries(SallenKeyFilter
    PRIVATE
        chowdsp::chowdsp_simd
)
