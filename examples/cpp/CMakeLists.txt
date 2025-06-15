# MIT License
#
# Copyright (c) 2025 Robert Schiwon
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# --------------------------------------------------------------
# General project settings
# --------------------------------------------------------------

# Works with 3.15 and newer CMake
cmake_minimum_required(VERSION 3.15...3.25)

# Project name and a few useful settings. Other commands can pick up the results
project(
    exampleapp
    VERSION 1.0.0
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
