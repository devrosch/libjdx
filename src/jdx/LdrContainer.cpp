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
#include "jdx/LdrContainer.hpp"
#include "util/LdrUtils.hpp"
#include "util/StringUtils.hpp"

std::optional<const std::string> libjdx::jdx::LdrContainer::parseStringValue(
    std::string& value, io::TextReader& reader)
{
    util::trim(value); // trim first line value only
    while (!reader.eof())
    {
        const auto line = reader.readLine();
        if (util::isLdrStart(line))
        {
            return line;
        }
        auto [content, comment] = util::stripLineComment(line);
        if (!content.empty() && !value.empty() && value.back() == '=')
        {
            // account for terminal "=" as non line breaking marker
            value.pop_back();
            value.append(line);
        }
        // Bruker quirk: check for specific comments that indicate the end of
        // the previous LDR
        else if (content.empty() && comment.has_value()
                 && (util::isBrukerSpecificSectionStart(line)
                     || util::isBrukerSpecificSectionEnd(line)))
        {
            return line;
        }
        else
        {
            value.append('\n' + line);
        }
    }
    return std::nullopt;
}
