set(SF_HEADER_LIST
    # none
)

set(SF_SOURCE_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/Wrapper.cpp"
)

target_sources("${SF_LIBRARY_NAME}"
    PRIVATE "${SF_SOURCE_LIST}"
    PUBLIC "${SF_HEADER_LIST}"
)
