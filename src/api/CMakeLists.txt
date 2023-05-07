set(API_HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/api/KeyValueParam.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Point2D.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Node.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/FileParser.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/FileParserSelector.hpp"

    "${PROJECT_SOURCE_DIR}/include/api/DataScanner.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/DataMapper.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Node2.hpp"
)

set(API_SOURCE_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/KeyValueParam.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Point2D.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Node.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/FileParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/FileParserSelector.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ExceptionUtils.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/DataScanner.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DataMapper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Node2.cpp"
)

target_sources("${SF_LIBRARY_NAME}"
    PRIVATE "${API_SOURCE_LIST}"
    PUBLIC "${API_HEADER_LIST}"
)
