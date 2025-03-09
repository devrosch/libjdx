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
#ifndef API_NODE_HPP
#define API_NODE_HPP

#include "api/KeyValueParam.hpp"
#include "api/Point2D.hpp"
#include "api/Table.hpp"

#include <string>
#include <vector>

namespace sciformats::api
{
struct Node
{
    std::string name;
    std::vector<KeyValueParam> parameters;
    std::vector<Point2D> data;
    std::map<std::string, std::string> metadata;
    Table table;
    std::vector<std::string> childNodeNames;
};
} // sciformats::api

#endif
