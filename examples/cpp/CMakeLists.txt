# --------------------------------------------------------------
# General project settings
# --------------------------------------------------------------

# Works with 3.15 and newer CMake
cmake_minimum_required(VERSION 3.15...3.25)

# Project name and a few useful settings. Other commands can pick up the results
project(
    exampleapp
    VERSION 0.1.0
    DESCRIPTION "A C++ example app for using libjdx for reading JCAMP-DX data."
    LANGUAGES CXX)

# --------------------------------------------------------------
# Options
# --------------------------------------------------------------

option(CMAKE_BUILD_TYPE "Build type. Defaults to Debug." Debug)
option(BUILD_SHARED_LIBS "Build shared of static library. Defaults to static." OFF)

# --------------------------------------------------------------
# Add sub directories contaning sources
# --------------------------------------------------------------

# The compiled library code is here
add_subdirectory("${PROJECT_SOURCE_DIR}/../.." build)

add_executable("${CMAKE_PROJECT_NAME}"
    Main.cpp
)

target_link_libraries("${CMAKE_PROJECT_NAME}" PUBLIC jdx)

# public headers
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${PROJECT_SOURCE_DIR}/../../include")
