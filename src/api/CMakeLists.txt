set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/api/KeyValueParam.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Point2D.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Node.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/FileParser.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/FileParserSelector.hpp"
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(api
    KeyValueParam.cpp
    Point2D.cpp
    Node.cpp
    FileParser.cpp
    FileParserSelector.cpp
    ${HEADER_LIST}
)

# All users of this library will need at least C++17
target_compile_features(api PUBLIC cxx_std_17)

if (EMSCRIPTEN)
    # make sure that code not used in this target is retained, see: https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
    # for making this work in CMake, see: https://stackoverflow.com/a/59994837
    # alternative discussion https://stackoverflow.com/questions/53071878/using-whole-archive-linker-option-with-cmake-and-libraries-with-other-library
    target_link_options(api INTERFACE "LINKER:--whole-archive")
endif()

# We need this directory, and users of our library will need it too
target_include_directories(api PUBLIC ${PROJECT_SOURCE_DIR}/include)
