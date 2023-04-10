set(IO_HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/io/BinaryReader.hpp"
)

set(IO_SOURCE_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/BinaryReader.cpp"
)

target_sources("${SF_LIBRARY_NAME}"
    PRIVATE "${IO_SOURCE_LIST}"
    PUBLIC "${IO_HEADER_LIST}"
)
