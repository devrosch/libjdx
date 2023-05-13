set(IO_HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/io/BinaryReader.hpp"
    "${PROJECT_SOURCE_DIR}/include/io/SimpleTextReader.hpp"
    "${PROJECT_SOURCE_DIR}/include/io/BufferedTextReader.hpp"
)

set(IO_SOURCE_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/BinaryReader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SimpleTextReader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/BufferedTextReader.cpp"
)

target_sources("${SF_LIBRARY_NAME}"
    PRIVATE "${IO_SOURCE_LIST}"
    PUBLIC "${IO_HEADER_LIST}"
)
