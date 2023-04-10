set(API_HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/api/KeyValueParam.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Point2D.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Node.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/FileParser.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/FileParserSelector.hpp"
    PARENT_SCOPE
)

set(API_SOURCE_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/KeyValueParam.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Point2D.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Node.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/FileParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/FileParserSelector.cpp"
    PARENT_SCOPE
)
