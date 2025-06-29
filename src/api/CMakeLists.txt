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

set(API_HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/api/Scanner.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Converter.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/ConverterService.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Node.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/KeyValueParam.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Point2D.hpp"
    "${PROJECT_SOURCE_DIR}/include/api/Table.hpp"
)

set(API_SOURCE_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/Scanner.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Converter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ConverterService.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Node.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/KeyValueParam.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Point2D.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ExceptionUtils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Table.cpp"
)

target_sources("${JDX_LIBRARY_NAME}"
    PRIVATE "${API_SOURCE_LIST}"
    PUBLIC "${API_HEADER_LIST}"
)
