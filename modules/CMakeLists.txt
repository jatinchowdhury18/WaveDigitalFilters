include(cmake/SubprojectVersion.cmake)
subproject_version(JUCE juce_version)
message(STATUS "VERSION for JUCE: ${juce_version}")

include(cmake/WarningFlags.cmake)
include(cmake/CreateWDFPlugin.cmake)
