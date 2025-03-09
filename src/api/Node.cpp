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
#include "api/Node.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Node)
{
    using namespace sciformats::api;
    using namespace emscripten;

    value_object<Node>("Node")
        .field("name", &Node::name)
        .field("parameters", &Node::parameters)
        .field("data", &Node::data)
        .field("table", &Node::table)
        .field("childNodeNames", &Node::childNodeNames)
        .field("metadata", &Node::metadata);

    register_vector<KeyValueParam>("vector<KeyValueParam>");
    register_vector<Point2D>("vector<Point2D>");
    register_vector<std::string>("vector<std::string>");
    // as an alternative for mapping vectors to JS arrays, see:
    // https://github.com/emscripten-core/emscripten/issues/11070
}
#endif
