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

# --------------------------------------------------------------
# Targets for creating code coverage reports
# --------------------------------------------------------------

# --------------------------------------------------------------
# Prerequisites
# --------------------------------------------------------------
# Requires lcov and genhtml command to be on the path
# compiler must be GNU g++ (i.e. CMAKE_COMPILER_IS_GNUCXX)
# build must be made in DEBUG mode and the following flags need to be set:
# add_compile_options(-g -O0 --coverage -fprofile-arcs -ftest-coverage)
# add_link_options(-lgcov --coverage -fprofile-arcs -ftest-coverage -p)

# --------------------------------------------------------------
# Run AFTER make and BEFORE executing tests to generate baseline
# --------------------------------------------------------------
add_custom_target(
    coverage-base
    COMMAND lcov
    --capture
    --initial
    --base-directory ${CMAKE_CURRENT_SOURCE_DIR}
    --directory ${CMAKE_CURRENT_BINARY_DIR}
    --output-file "${CMAKE_CURRENT_BINARY_DIR}/coverage_base.info"
)

# --------------------------------------------------------------
# Run AFTER executing tests to report coverage
# --------------------------------------------------------------
add_custom_target(
    coverage-capture
    COMMAND lcov --capture
    --base-directory ${CMAKE_CURRENT_SOURCE_DIR}
    --directory ${CMAKE_CURRENT_BINARY_DIR}
    --output-file "${CMAKE_CURRENT_BINARY_DIR}/coverage.info"
    # combine coverage baseline with captured results
    COMMAND lcov
    --add-tracefile "${CMAKE_CURRENT_BINARY_DIR}/coverage_base.info"
    --add-tracefile "${CMAKE_CURRENT_BINARY_DIR}/coverage.info"
    --output-file "${CMAKE_CURRENT_BINARY_DIR}/coverage_total.info"
    # remove system and test code from coverage
    COMMAND lcov
    --remove "${CMAKE_CURRENT_BINARY_DIR}/coverage_total.info"
    # paths to be removed must be enclosed by SINGLE quotes
    '/usr/*' '*/lib/*' '*/tests/*'
    --output-file "${CMAKE_CURRENT_BINARY_DIR}/coverage_filtered.info"
)

# --------------------------------------------------------------
# Run AFTER coverage-capture to generate HTML report
# --------------------------------------------------------------
add_custom_target(
    coverage-report
    COMMAND genhtml
    "${CMAKE_CURRENT_BINARY_DIR}/coverage_filtered.info"
    --output-directory "${CMAKE_CURRENT_BINARY_DIR}/coverage"
)
