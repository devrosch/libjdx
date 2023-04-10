# Tests need to be added as executables
add_executable(stub_test
    StubFileParserTest.cpp
    StubNodeTest.cpp
)

# Link to the library, as well as the Catch2 testing library
target_link_libraries(stub_test PRIVATE "${SF_LIBRARY_NAME}" catch2)

# https://stackoverflow.com/questions/12313258/cmake-how-to-install-test-files-with-unit-tests
# https://stackoverflow.com/questions/34799916/copy-file-from-source-directory-to-binary-directory-using-cmake
add_custom_command(
    TARGET stub_test POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
            "$<TARGET_FILE_DIR:stub_test>/resources/"
    VERBATIM # use VERBATIM to correctly handle paths that include spaces
)

add_custom_command(
    TARGET stub_test POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E echo
    "target dir stub_test: $<TARGET_FILE_DIR:stub_test>")

if (EMSCRIPTEN)
    # Embed test files, make available in sub directory "resources" (specified after @)
    target_link_options(stub_test PRIVATE "SHELL:--embed-file ${CMAKE_CURRENT_SOURCE_DIR}/resources@resources")
endif()

# Register test for ctest
if (EMSCRIPTEN)
    # run test with node.js
    add_test(NAME stub_test COMMAND node "${CMAKE_CURRENT_BINARY_DIR}/stub_test.js")
else()
    # run natively
    add_test(NAME stub_test COMMAND stub_test) # Command can be a target
endif()
