#set(HEADER_LIST
#	"${binary_reader_SOURCE_DIR}/include/binaryreader/binary_reader.hpp"
#	"${binary_reader_SOURCE_DIR}/include/catch2/catch.hpp")

# Tests need to be added as executables first
#add_executable(binary_reader_test binary_reader_test.cpp ${HEADER_LIST})
add_executable(io_test BinaryReaderTest.cpp)

# I'm using C++17 in the test
target_compile_features(io_test PRIVATE cxx_std_17)

# We need this directory, and users of our library will need it too
#target_include_directories(io_test PRIVATE ${INCLUDE_DIRECTORIES} ${TESTING_INCLUDE_DIRECTORIES})

# Should be linked to the stream_reader library, as well as the Catch2 testing library
target_link_libraries(io_test PRIVATE sf catch2)

# https://stackoverflow.com/questions/12313258/cmake-how-to-install-test-files-with-unit-tests
# https://stackoverflow.com/questions/34799916/copy-file-from-source-directory-to-binary-directory-using-cmake
add_custom_command(
        TARGET io_test POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
                "${CMAKE_CURRENT_BINARY_DIR}/resources/"
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
