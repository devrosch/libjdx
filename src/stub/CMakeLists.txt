set(STUB_HEADER_LIST
    # public api
    "${PROJECT_SOURCE_DIR}/include/stub/StubFileParser.hpp"
    "${PROJECT_SOURCE_DIR}/include/stub/StubNode.hpp"
)

set(STUB_SOURCE_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/StubFileParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/StubNode.cpp"
)

target_sources("${SF_LIBRARY_NAME}"
    PRIVATE "${STUB_SOURCE_LIST}"
    PUBLIC "${STUB_HEADER_LIST}"
)
