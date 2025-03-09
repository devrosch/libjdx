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

target_sources("${JDX_LIBRARY_NAME}"
    PRIVATE "${IO_SOURCE_LIST}"
    PUBLIC "${IO_HEADER_LIST}"
)
