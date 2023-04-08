set(HEADER_LIST
    # public api
    "${PROJECT_SOURCE_DIR}/include/stub/StubFileParser.hpp"
    "${PROJECT_SOURCE_DIR}/include/stub/StubNode.hpp"
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(stub
    # public api
    StubFileParser.cpp
    StubNode.cpp
)

target_link_libraries(stub PUBLIC api)

# All users of this library will need at least C++17
target_compile_features(stub PUBLIC cxx_std_17)
# For g++ 12 the above does not seem to suffice (e.g. does not recognize std::optional), hence additionally setting properties
set_target_properties(stub PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

# public headers
target_include_directories(stub PUBLIC ${PROJECT_SOURCE_DIR}/include)
