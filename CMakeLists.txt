cmake_minimum_required(VERSION 3.15)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment target")
project(WaveDigitalFilters VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(modules)

add_subdirectory(VoltageDivider)
add_subdirectory(CurrentDivider)
add_subdirectory(PassiveLPF)
add_subdirectory(LCOscillator)
