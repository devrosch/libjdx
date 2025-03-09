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

# For WASM, create an executable of same name as library
# Only this will generate *.wasm and *.js files (e.g., libjdx.wasm and libjdx.js) that wrap the library and can be uses from JS
add_executable("${CMAKE_PROJECT_NAME}"
    Main.cpp
)

# Should be linked to the library
target_link_libraries("${CMAKE_PROJECT_NAME}" PUBLIC "${JDX_LIBRARY_NAME}")

target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE JDX_PROJECT_NAME="${PROJECT_NAME}")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE JDX_PROJECT_VERSION="${PROJECT_VERSION}")

if (EMSCRIPTEN)
    # Emscripten
    # Avoid "undefined symbol" errors, see: https://stackoverflow.com/a/37869647
    target_link_options(libjdx PRIVATE "--bind")
    # Link workerfs
    target_link_options(libjdx PRIVATE "-lworkerfs.js")
endif()
