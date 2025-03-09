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
#ifndef IO_STRINGPREFIXTYPE_HPP
#define IO_STRINGPREFIXTYPE_HPP

#include "Endianness.hpp"
#include "StringPrefixNumericType.hpp"

namespace libjdx::io
{
/**
 * @brief The configuration for reading a string.
 */
struct StringPrefixType
{
    /**
     * @brief numericType The numeric type of the prefix, including the number
     * of bytes. Default: Int16Chars16.
     */
    StringPrefixNumericType numericType = StringPrefixNumericType::Int16Chars16;
    /**
     * @brief prefixEndianness The endianness of the prefix chars. Only relevant
     * if prefix consist of more than one char. Default: LittleEndian.
     */
    Endianness endianness = Endianness::LittleEndian;
};

} // namespace libjdx::io

#endif // IO_STRINGPREFIXTYPE_HPP
