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
#ifndef JDX_STRINGUTILS_HPP
#define JDX_STRINGUTILS_HPP

#include <string>
#include <vector>

/**
 * @brief Helper functions for processing strings.
 */
namespace sciformats::jdx::util
{
void trim(std::string& s);
void trimLeft(std::string& s);
void trimRight(std::string& s);
bool isSpace(char c);
void toLower(std::string& s);
std::vector<std::string> split(const std::string& input,
    const std::string& delimiterRegEx, bool trimSegments = false,
    size_t matchGroup = 0);
} // namespace sciformats::jdx::utils

#endif // JDX_STRINGUTILS_HPP
