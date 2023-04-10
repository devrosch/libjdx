set(STUB_HEADER_LIST
    # public api
    "${PROJECT_SOURCE_DIR}/include/stub/StubFileParser.hpp"
    "${PROJECT_SOURCE_DIR}/include/stub/StubNode.hpp"
    PARENT_SCOPE
)

set(STUB_SOURCE_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/StubFileParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/StubNode.cpp"
    PARENT_SCOPE
)
