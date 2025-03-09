# Copyright (C) 2025 Robert Schiwon
#
# This file is part of libjdx.
#
# libjdx is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option) any
# later version.
#
# libjdx is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with libjdx. If not, see <https://www.gnu.org/licenses/>.

set(JDX_PUBLIC_HEADER_LIST
    # public api
    "${PROJECT_SOURCE_DIR}/include/jdx/api/JdxScanner.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/api/JdxConverter.hpp"
    # public data providers
    "${PROJECT_SOURCE_DIR}/include/jdx/JdxParser.hpp"
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
    # Bruker
    "${PROJECT_SOURCE_DIR}/include/jdx/BrukerSpecificParameters.hpp"
    "${PROJECT_SOURCE_DIR}/include/jdx/BrukerRelaxSection.hpp"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/api/JdxScanner.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/api/JdxConverter.cpp"
    # public data providers
    "${CMAKE_CURRENT_SOURCE_DIR}/JdxParser.cpp"
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
    # Bruker
    "${CMAKE_CURRENT_SOURCE_DIR}/BrukerSpecificParameters.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/BrukerRelaxSection.cpp"
)

target_sources("${JDX_LIBRARY_NAME}"
    PRIVATE "${JDX_SOURCE_LIST}"
    PUBLIC "${JDX_PUBLIC_HEADER_LIST}"
    PRIVATE "${JDX_PRIVATE_HEADER_LIST}"
)
