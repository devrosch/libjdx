/* Copyright (C) 2025 Robert Schiwon
 *
 * This file is part of libjdx.
 *
 * libjdx is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * libjdx is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libjdx. If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef API_TABLE_HPP
#define API_TABLE_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace sciformats::api
{
/**
 * A table.
 */
struct Table
{
    /**
     * A list of column keys and corresponding column names.
     */
    std::vector<std::pair<std::string, std::string>> columnNames;

    /**
     * A list of rows.
     *
     * Each key-value pair in the map represents a single cell,
     * e.g., peak parameters such as position or height.
     * Only keys from the coulmnNameMap may occur but not all keys from
     * that list need to occur as there may be missing values for a peak.
     */
    std::vector<std::map<std::string, std::string>> rows;
};
} // sciformats::api

#endif
