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
add_executable(jdx_test
    api/JdxScannerTest.cpp
    api/JdxConverterTest.cpp
    JdxParserTest.cpp
    StringUtilsTest.cpp
    LdrUtilsTest.cpp
    StringLdrTest.cpp
    BlockTest.cpp
    DataParserTest.cpp
    XyDataTest.cpp
    RaDataTest.cpp
    XyPointsTest.cpp
    PeakTableTest.cpp
    PeakAssignmentsTest.cpp
    NTuplesTest.cpp
    IntegrationTest.cpp
    AuditTrailTest.cpp
    BrukerRelaxSectionTest.cpp
    BrukerSpecificParametersTest.cpp
)

# Link to the library, as well as the Catch2 testing library
target_link_libraries(jdx_test PRIVATE "${JDX_LIBRARY_NAME}" catch2)
# include private headers see https://stackoverflow.com/questions/35385908/cmake-testing-a-library-header-location-issue
# required for testing some utils
target_include_directories(jdx_test PRIVATE $<TARGET_PROPERTY:jdx,INCLUDE_DIRECTORIES>)

# https://stackoverflow.com/questions/12313258/cmake-how-to-install-test-files-with-unit-tests
# https://stackoverflow.com/questions/34799916/copy-file-from-source-directory-to-binary-directory-using-cmake
add_custom_command(
    TARGET jdx_test POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
            "$<TARGET_FILE_DIR:jdx_test>/resources/"
# below works with gcc and clang but does not work with VS an Xcode as they create sub folder per build config (Debug, Release, ...)
#            "${CMAKE_CURRENT_BINARY_DIR}/resources/"
    VERBATIM # use VERBATIM to correctly handle paths that include spaces
)

add_custom_command(
    TARGET jdx_test POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E echo
    "target dir jdx_test: $<TARGET_FILE_DIR:jdx_test>")

if (EMSCRIPTEN)
    # Embed test files, make available in sub directory "resources" (specified after @)
    target_link_options(jdx_test PRIVATE "SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/resources@resources")
endif()

# Register test for ctest
if (EMSCRIPTEN)
    # run test with node.js
    add_test(NAME jdx_test COMMAND node "${CMAKE_CURRENT_BINARY_DIR}/jdx_test.js")
else()
    # run natively
    add_test(NAME jdx_test COMMAND jdx_test) # Command can be a target
endif()
