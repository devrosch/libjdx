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

# Tests need to be added as executables
add_executable(io_test
    BinaryReaderTest.cpp    
    BufferedTextReaderTest.cpp
    SimpleTextReaderTest.cpp
)

# Link to the library, as well as the Catch2 testing library
target_link_libraries(io_test PRIVATE "${JDX_LIBRARY_NAME}" catch2)

# https://stackoverflow.com/questions/12313258/cmake-how-to-install-test-files-with-unit-tests
# https://stackoverflow.com/questions/34799916/copy-file-from-source-directory-to-binary-directory-using-cmake
add_custom_command(
        TARGET io_test POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
                "$<TARGET_FILE_DIR:io_test>/resources/"
        VERBATIM # use VERBATIM to correctly handle paths that include spaces
)

if (EMSCRIPTEN)
    # Embed test files, make available in sub directory "resources" (specified after @)
    target_link_options(io_test PRIVATE "SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/resources@resources")
endif()

# Register test for ctest
if (EMSCRIPTEN)
    # run test with node.js
    add_test(NAME io_test COMMAND node "${CMAKE_CURRENT_BINARY_DIR}/io_test.js")
else()
    # run natively
    add_test(NAME io_test COMMAND io_test) # Command can be a target
endif()
