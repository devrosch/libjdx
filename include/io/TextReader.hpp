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
#ifndef IO_TEXTREADER_HPP
#define IO_TEXTREADER_HPP

#ifdef __EMSCRIPTEN__
#include "io/BufferedTextReader.hpp"
#else
#include "io/SimpleTextReader.hpp"
#endif

namespace sciformats::io
{
// use BufferedTextReader for EMSCRIPTEN build as it drastically improves data
// reading performance in browser context
#ifdef __EMSCRIPTEN__
typedef BufferedTextReader TextReader;
#else
typedef SimpleTextReader TextReader;
#endif
}

#endif
