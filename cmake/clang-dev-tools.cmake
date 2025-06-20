# Copyright (C) 2025 Robert Schiwon
#
# This file is part of libjdx.
#
# libjdx is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# libjdx is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with libjdx. If not, see <https://www.gnu.org/licenses/>.

# --------------------------------------------------------------
# Targets for running clang-tidy and clang-format
# Requires clang-format (^10.0.0) and clang-tidy (^6.0.0) to be on the path
# see: https://stackoverflow.com/questions/32280717/cmake-clang-tidy-or-other-script-as-custom-target
# --------------------------------------------------------------

# --------------------------------------------------------------
# Collect sources info for clang-tidy and clang-format
# --------------------------------------------------------------

# export json database of processed files for use by clang-tidy during build
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# include directories for use with clang-tidy and make as separate target
# set(INCLUDE_DIRECTORIES
#     "${PROJECT_SOURCE_DIR}/include"
#     "${PROJECT_SOURCE_DIR}/src"
#     "${PROJECT_SOURCE_DIR}/lib/catch2/include"
#     "${PROJECT_SOURCE_DIR}/lib/trompeloeil/include"
# )
# source files for use with clang-tidy and make as separate target
file(GLOB_RECURSE ALL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/apps/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.hpp"
    "${PROJECT_SOURCE_DIR}/tests/*.cpp"
    "${PROJECT_SOURCE_DIR}/include/*.hpp")
#message(STATUS "clang-tidy INCLUDE_DIRECTORIES: ${INCLUDE_DIRECTORIES}")
message(STATUS "clang-tidy/clang-format ALL_SOURCE_FILES: ${ALL_SOURCE_FILES}")

# prepend include directories with -I to pass them as multiple options to make
#set(CLANG_TIDY_MAKE_INCLUDES ${INCLUDE_DIRECTORIES})
#list(TRANSFORM CLANG_TIDY_MAKE_INCLUDES PREPEND -I)
#message(STATUS "CLANG_TIDY_MAKE_INCLUDES: ${CLANG_TIDY_MAKE_INCLUDES}")

# --------------------------------------------------------------
# Add targets
# --------------------------------------------------------------

add_custom_target(
        clang-tidy
        VERBATIM # use VERBATIM to correctly handle paths that include spaces
        COMMAND clang-tidy
        --extra-arg=-std=c++17
        -p ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
        --checks=${CLANG_TIDY_CHECKS}
        --warnings-as-errors=${CLANG_TIDY_CHECKS}
        ${ALL_SOURCE_FILES}
        #--
        #-std=c++17
        #${CLANG_TIDY_MAKE_INCLUDES}
)

add_custom_target(
        clang-format-check
        COMMAND clang-format
        --style=file
        --dry-run
        --Werror
        ${ALL_SOURCE_FILES}
)

add_custom_target(
        clang-format-fix
        COMMAND clang-format
        --style=file
        -i
        ${ALL_SOURCE_FILES}
)
