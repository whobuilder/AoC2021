cmake_minimum_required(VERSION 3.0.2)
project(adventofcode CXX)

include(${ONKI_CMAKE_FILES_PATH}/StandardProjectSettings.cmake)
include(${ONKI_CMAKE_FILES_PATH}/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)
# standard compiler warnings
include(${ONKI_CMAKE_FILES_PATH}/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(${ONKI_CMAKE_FILES_PATH}/Sanitizers.cmake)
enable_sanitizers(project_options)
target_compile_features(project_options INTERFACE cxx_std_17)

find_package(range-v3 REQUIRED)
find_package(fmt REQUIRED)
add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)
add_subdirectory(day5)
add_subdirectory(day6)
add_subdirectory(day7)
add_subdirectory(day8)
add_subdirectory(day9)
add_subdirectory(day10)
add_subdirectory(day11)
add_subdirectory(day12)
add_subdirectory(day13)
add_subdirectory(day14)
