set(JDX_PUBLIC_HEADER_LIST
    # public api
    "${PROJECT_SOURCE_DIR}/include/jdx/api/JdxBlockNode.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/api/JdxData2DNode.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/api/JdxFileParser.hpp"

    "${PROJECT_SOURCE_DIR}/include/jdx/api/JdxDataScanner.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/api/JdxDataMapper.hpp"

    # public data providers
    "${PROJECT_SOURCE_DIR}/include/jdx/JdxParser.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/TextReader.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/SimpleTextReader.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/BufferedTextReader.hpp"

    # ldr
    "${PROJECT_SOURCE_DIR}/include/jdx/LdrContainer.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/Block.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/Ldr.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/StringLdr.hpp"
    # data
    "${PROJECT_SOURCE_DIR}/include/jdx/DataLdr.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/NTuples.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/NTuplesAttributes.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/Page.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/DataTable.hpp"
    # 2D data
    "${PROJECT_SOURCE_DIR}/include/jdx/XyParameters.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/RaParameters.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/Data2D.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/XyData.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/RaData.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/XyPoints.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/XyBase.hpp"
    # tabular data
    "${PROJECT_SOURCE_DIR}/include/jdx/TabularData.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/Peak.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/PeakTable.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/PeakAssignment.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/PeakAssignments.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/AuditTrail.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/AuditTrailEntry.hpp"
    # exception
    "${PROJECT_SOURCE_DIR}/include/jdx/ParseException.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/BlockParseException.hpp"
)

set(JDX_PRIVATE_HEADER_LIST
    # util (private headers)
    "${CMAKE_CURRENT_SOURCE_DIR}/util/StringUtils.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/LdrUtils.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/DataParser.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/TuplesParser.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/PeakTableParser.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/MultilineTuplesParser.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/PeakAssignmentsParser.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/AuditTrailParser.hpp"
)

set(JDX_SOURCE_LIST
    # public api
    "${CMAKE_CURRENT_SOURCE_DIR}/api/JdxBlockNode.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/api/JdxData2DNode.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/api/JdxFileParser.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/api/JdxDataScanner.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/api/JdxDataMapper.cpp"

    # public data providers
    "${CMAKE_CURRENT_SOURCE_DIR}/JdxParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SimpleTextReader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/BufferedTextReader.cpp"

    # util
    "${CMAKE_CURRENT_SOURCE_DIR}/util/StringUtils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/LdrUtils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/DataParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/TuplesParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/PeakTableParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/MultilineTuplesParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/PeakAssignmentsParser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/AuditTrailParser.cpp"
    # ldr
    "${CMAKE_CURRENT_SOURCE_DIR}/Block.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/LdrContainer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Ldr.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/StringLdr.cpp"
    # data
    "${CMAKE_CURRENT_SOURCE_DIR}/DataLdr.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/NTuples.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Page.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DataTable.cpp"
    # 2D data
    "${CMAKE_CURRENT_SOURCE_DIR}/Data2D.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/XyData.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RaData.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/XyPoints.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/XyBase.cpp"
    # tabular data
    "${CMAKE_CURRENT_SOURCE_DIR}/TabularData.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/PeakTable.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/PeakAssignments.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/AuditTrail.cpp"
    # exception
    "${CMAKE_CURRENT_SOURCE_DIR}/ParseException.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/BlockParseException.cpp"
)

target_sources("${SF_LIBRARY_NAME}"
    PRIVATE "${JDX_SOURCE_LIST}"
    PUBLIC "${JDX_PUBLIC_HEADER_LIST}"
    PRIVATE "${JDX_PRIVATE_HEADER_LIST}"
)
