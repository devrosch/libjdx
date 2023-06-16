set(API_HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/api/Scanner.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Converter.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/ConverterService.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Node.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/KeyValueParam.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Point2D.hpp"
)

set(API_SOURCE_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/Scanner.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Converter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ConverterService.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Node.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/KeyValueParam.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Point2D.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ExceptionUtils.cpp"
)

target_sources("${SF_LIBRARY_NAME}"
    PRIVATE "${API_SOURCE_LIST}"
    PUBLIC "${API_HEADER_LIST}"
)
