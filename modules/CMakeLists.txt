add_subdirectory(JUCE)
include(cmake/SubprojectVersion.cmake)
subproject_version(JUCE juce_version)
message(STATUS "VERSION for JUCE: ${juce_version}")

# add custom JUCE modules
add_subdirectory(chowdsp_utils)
add_subdirectory(chowdsp_wdf)

include(cmake/WarningFlags.cmake)
include(cmake/CreateWDFPlugin.cmake)
