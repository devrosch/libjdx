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
#ifndef IO_STRINGPREFIXNUMERICTYPE_HPP
#define IO_STRINGPREFIXNUMERICTYPE_HPP

#include <cstdint> // for uint8_t

namespace libjdx::io
{
/**
 * @brief The endianness indicates the byte order of data.
 */
enum class StringPrefixNumericType : uint8_t
{
    Int8Chars8, ///< One signed byte with length of uint8_t characters
    UInt8Chars8, ///< One unsigned byte, with length of uint8_t characters
    Int8Chars16, ///< One signed byte, with length of uint16_t characters
    UInt8Chars16, ///< One unsigned byte, with length of uint16_t characters
    Int16Chars8, ///< Two signed bytes, with length of uint8_t characters
    UInt16Chars8, ///< Two unsigned bytes, with length of uint8_t characters
    Int16Chars16, ///< Two signed bytes, with length of uint16_t characters
    UInt16Chars16, ///< Two unsigned bytes, with length of uint16_t characters

    // this could lead to extremely large strings,
    // would require additional checks during parsing
    // Int32Chars8, ///< Four signed bytes, with length of uint8_t characters
    // Int32Chars16, ///< Four signed bytes, with length of uint16_t characters

    // this excedds possible ICU string length
    // UInt32Chars8, ///< Four unsigned bytes, with length of uint8_t characters
    // UInt32Chars16, ///< Four unsigned bytes, with length of uint16_t
    // characters
};

} // namespace libjdx::io

#endif // IO_STRINGPREFIXNUMERICTYPE_HPP
