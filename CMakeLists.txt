# --------------------------------------------------------------
# General project settings
# --------------------------------------------------------------

# Works with 3.15 and newer CMake
cmake_minimum_required(VERSION 3.15...3.25)

# Project name and a few useful settings. Other commands can pick up the results
project(
    libsf
    VERSION 0.1
    DESCRIPTION "A library for reading scientific data formats"
    LANGUAGES CXX)

# For portability, do not use compiler specific C++ extensions and require minimum C++ standard
# However, if included by another project that decision should be made there, so only make setting if top level project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # E.g., for g++, let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)
    # Set C++ 17
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()


# --------------------------------------------------------------
# Options
# --------------------------------------------------------------

option(CMAKE_BUILD_TYPE "Build type. Defaults to Debug" Debug)
option(BUILD_SHARED_LIBS "Build shared of static library. Defaults to static" OFF)
option(COVERAGE "Capture test coverage. Only relevant if CMAKE_CXX_COMPILER=g++ and CMAKE_BUILD_TYPE=Debug" ON)
option(CMAKE_EXPORT_COMPILE_COMMANDS "generate compile_commands.json" ON)

# --------------------------------------------------------------
# Add Emscripten flags
# --------------------------------------------------------------

if (EMSCRIPTEN)
    # TODO: also set USE_ICU through compile/link options
    # TODO: (only) set in io library that actually requires ICU
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_ICU=1")
    # use SHELL prefix to make sure -s flag is not de-duplicated
    # see: https://cmake.org/cmake/help/latest/command/add_compile_options.html#option-de-duplication
    add_compile_options(SHELL:-s DISABLE_EXCEPTION_CATCHING=0)
    add_link_options(SHELL:-s DISABLE_EXCEPTION_CATCHING=0)
endif()

# --------------------------------------------------------------
# Add code coverage support
# --------------------------------------------------------------

if (COVERAGE AND CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE MATCHES Debug)
    # we want to capture test covergae, use g++ and do a debug build
    message(STATUS "setting flags for capturing test coverage")
    add_compile_options(-g -O0 --coverage -fprofile-arcs -ftest-coverage)
    add_link_options(-lgcov --coverage -fprofile-arcs -ftest-coverage -p)
    message(STATUS "set compile options: -g -O0 --coverage -fprofile-arcs -ftest-coverage")
    message(STATUS "set link options: -lgcov --coverage -fprofile-arcs -ftest-coverage -p")
endif()

# --------------------------------------------------------------
# Run clang-tidy during build and create separate target
# --------------------------------------------------------------

# clang-tidy rules to check
# deactivate checks for magic-numbers as they are
# too sensitive in bit oprations and sample data
# bugprone-unchecked-optional-access, misc-const-correctness, readability-identifier-length deactivated for now as they generate to many warnings
# TODO: reactivate and fix bugprone-unchecked-optional-access, misc-const-correctness, readability-identifier-length
set(CLANG_TIDY_CHECKS
    "-*\
    ,bugprone-*,-bugprone-unchecked-optional-access,-bugprone-easily-swappable-parameters\
    ,cert-*\
    ,clang-analyzer-*\
    ,cppcoreguidelines-*,-cppcoreguidelines-avoid-magic-numbers\
    ,hicpp-*,misc-*.-misc-const-correctness\
    ,modernize-*,-modernize-use-trailing-return-type\
    ,performance-*\
    ,portability-*\
    ,readability-*,-readability-magic-numbers,-readability-identifier-length")

find_program(CLANG_TIDY_FOUND "clang-tidy")
if(CLANG_TIDY_FOUND AND NOT EMSCRIPTEN)
#if(NOT EMSCRIPTEN)
# alternatively: if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "[Ee]mscripten")
#if(UNIX AND NOT EMSCRIPTEN AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # activate clang-tidy for all targets when available and not compiling with EMSCRIPTEN as it runs into issues with clang-tidy
    # see: https://stackoverflow.com/questions/40433573/how-can-i-specify-additional-arguments-for-use-with-cmake-cxx-clang-tidy-variabl
    set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=${CLANG_TIDY_CHECKS})
    message(STATUS "CMAKE_CXX_CLANG_TIDY: ${CMAKE_CXX_CLANG_TIDY}")
endif()

# Including cmake script to create clang-tidy and clang-format targets
# see: https://stackoverflow.com/questions/32280717/cmake-clang-tidy-or-other-script-as-custom-target
# This allows to run "make clan-tidy" and "make clang-format" in the build directory
# Alternatively, in the build directory, the "run-clang-tidy.py" could be used, but requires python
# Only include if this is the main app (and not included through add_subdirectory)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(cmake/clang-dev-tools.cmake)
endif()

# --------------------------------------------------------------
# Add sub directories contaning sources
# --------------------------------------------------------------

# The compiled library code is here
add_subdirectory(src)
# The executable code is here
if(EMSCRIPTEN)
    # required, so that CMake generates *.js and *.wasm files for library
    add_subdirectory(apps)
endif()

# --------------------------------------------------------------
# Add code for testing
# --------------------------------------------------------------

# Make testing available if this is the main app (and not included through add_subdirectory)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    include(CTest)
endif()

# Make testing available if this is the main app
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING)
    add_subdirectory(lib/catch2)
    add_subdirectory(lib/trompeloeil)
    add_subdirectory(tests)
endif()

# Including cmake script to create coverage-prepare and coverage targets
# Only include if this is the main app (and not included through add_subdirectory) and COVERAGE option is on
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING AND COVERAGE)
    include(cmake/coverage.cmake)
endif()

# --------------------------------------------------------------
# Set correct paths in doxygen config
# --------------------------------------------------------------
configure_file("${PROJECT_SOURCE_DIR}/doxy.config.in" "${PROJECT_BINARY_DIR}/doxy.config")
