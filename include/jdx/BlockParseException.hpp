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
#ifndef JDX_BLOCKPARSEEXCEPTION_HPP
#define JDX_BLOCKPARSEEXCEPTION_HPP

#include "jdx/ParseException.hpp"

#include <string>

namespace sciformats::jdx
{
/**
 * @brief Indicates an exception during parsing of JCAMP-DX data.
 */
class BlockParseException : public ParseException
{
public:
    explicit BlockParseException(const std::string& what);

    BlockParseException(const std::string& issueMsg, const std::string& label,
        const std::string& blockTitle);
};
} // namespace sciformats::jdx

#endif // JDX_BLOCKPARSEEXCEPTION_HPP
