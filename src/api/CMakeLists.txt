set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/api/model/KeyValueParam.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/model/Point2D.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/model/Node.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/model/FileParser.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/model/FileParserSelector.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/stub/StubNode.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/stub/StubFileParser.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/jdx/JdxBlockNode.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/jdx/JdxData2DNode.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/jdx/JdxFileParser.hpp"
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(sciwrap
    KeyValueParam.cpp
    Point2D.cpp
    Node.cpp
    FileParser.cpp
    FileParserSelector.cpp
    StubNode.cpp
    StubFileParser.cpp
    JdxBlockNode.cpp
    JdxData2DNode.cpp
    JdxFileParser.cpp
    ${HEADER_LIST}
)

target_link_libraries(sciwrap PRIVATE jdxparser)

# All users of this library will need at least C++17
target_compile_features(sciwrap PUBLIC cxx_std_17)

if (EMSCRIPTEN)
    # make sure that code not used in this target is retained, see: https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
    # for making this work in CMake, see: https://stackoverflow.com/a/59994837
    # alternative discussion https://stackoverflow.com/questions/53071878/using-whole-archive-linker-option-with-cmake-and-libraries-with-other-library
    target_link_options(sciwrap INTERFACE "LINKER:--whole-archive")
endif()

# We need this directory, and users of our library will need it too
target_include_directories(sciwrap PUBLIC ${PROJECT_SOURCE_DIR}/include/api)

# TODO: just for testing
# include private headers see https://stackoverflow.com/questions/35385908/cmake-testing-a-library-header-location-issue
# required for testing some utils
target_include_directories(sciwrap PRIVATE $<TARGET_PROPERTY:jdxparser,INCLUDE_DIRECTORIES>)
