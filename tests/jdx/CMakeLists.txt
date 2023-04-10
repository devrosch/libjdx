# Tests need to be added as executables
add_executable(jdx_test
    api/JdxFileParserTest.cpp
    TextReaderTest.cpp
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
    )

# Link to the library, as well as the Catch2 testing library
target_link_libraries(jdx_test PRIVATE sf catch2)
# include private headers see https://stackoverflow.com/questions/35385908/cmake-testing-a-library-header-location-issue
# required for testing some utils
target_include_directories(jdx_test PRIVATE $<TARGET_PROPERTY:sf,INCLUDE_DIRECTORIES>)

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
