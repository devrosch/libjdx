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

set(WRAPPER_HEADER_LIST
    # none
)

set(WRAPPER_SOURCE_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/Wrapper.cpp"
)

target_sources("${JDX_LIBRARY_NAME}"
    PRIVATE "${WRAPPER_SOURCE_LIST}"
    PUBLIC "${WRAPPER_HEADER_LIST}"
)
