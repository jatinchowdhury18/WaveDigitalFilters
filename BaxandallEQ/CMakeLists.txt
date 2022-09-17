create_wdf_plugin(BaxandallEQ BaEQ)

target_sources(BaxandallEQ PRIVATE
    src/BaxandallWDF.cpp
    src/BaxandallWDFAdapt.cpp
)

target_link_libraries(BaxandallEQ
    PRIVATE
        chowdsp::chowdsp_simd
)
