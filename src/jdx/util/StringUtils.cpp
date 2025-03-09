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
#include "util/StringUtils.hpp"
#include "jdx/ParseException.hpp"

#include <algorithm>
#include <functional>
#include <regex>
#include <string>

void libjdx::jdx::util::trimLeft(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not_fn(isSpace)));
}

void libjdx::jdx::util::trimRight(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not_fn(isSpace)).base(),
        s.end());
}

void libjdx::jdx::util::trim(std::string& s)
{
    trimRight(s);
    trimLeft(s);
}

bool libjdx::jdx::util::isSpace(char c)
{
    return static_cast<bool>(std::isspace(static_cast<unsigned char>(c)));
}

void libjdx::jdx::util::toLower(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
}

std::vector<std::string> libjdx::jdx::util::split(const std::string& input,
    const std::string& delimiterRegEx, bool trimSegments, size_t matchGroup)
{
    auto remainder = input;
    std::regex delimiter{delimiterRegEx};
    std::smatch match;
    std::vector<std::string> output;
    while (std::regex_search(remainder, match, delimiter))
    {
        auto matchPos = static_cast<size_t>(match.position(matchGroup));
        std::string segment = remainder.substr(0, matchPos);
        output.push_back(segment);
        auto nextPos
            = matchPos + static_cast<size_t>(match[matchGroup].length());
        remainder = remainder.substr(nextPos);
    }
    output.push_back(remainder);

    if (trimSegments)
    {
        std::for_each(output.begin(), output.end(), trim);
    }

    return output;
}
