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
#ifndef IO_ENDIANNESS_HPP
#define IO_ENDIANNESS_HPP

#include <cstdint> // for uint8_t

namespace sciformats::io
{
/**
 * @brief The endianness indicates the byte order of data.
 */
enum class Endianness : uint8_t
{
    LittleEndian, ///< The least significant byte comes first.
    BigEndian, ///< The most significant byte comes first.
};

} // namespace sciformats::io

#endif // IO_ENDIANNESS_HPP
