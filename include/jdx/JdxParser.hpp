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
#ifndef JDX_JDXPARSER_HPP
#define JDX_JDXPARSER_HPP

#include "jdx/Block.hpp"

#include <istream>
#include <memory>
#include <string>

namespace libjdx::jdx
{
/**
 * @brief A parser for JCAMP-DX data.
 */
class JdxParser
{
public:
    /**
     * @brief Shallow check if the data can be parsed, e.g. by checking the file
     * extension or magic bytes.
     * @param filePath Path to the file.
     * @param iStream Binary input stream.
     * @return A tentative indication whether this parser can process the file.
     */
    bool static canParse(const std::string& filePath, std::istream& iStream);

    /**
     * @brief Parses the data.
     * @param stream Binary input stream for the data.
     * @return A Block representing the data.
     */
    Block static parse(std::unique_ptr<std::istream> streamPtr);

    /**
     * @brief Parses the data.
     * @param filePath Path to the file.
     * @return A Block representing the data.
     */
    Block static parse(const std::string& filePath);

private:
    static constexpr std::array<const char*, 3> s_acceptedExtensions
        = {".jdx", ".dx", ".jcm"};
    static constexpr size_t s_scanBufferSize = 16;
};
} // namespace libjdx::jdx

#endif // JDX_JDXPARSER_HPP
