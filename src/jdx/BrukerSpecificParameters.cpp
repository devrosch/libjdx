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
#include "jdx/BrukerSpecificParameters.hpp"
#include "jdx/ParseException.hpp"
#include "util/LdrUtils.hpp"

libjdx::jdx::BrukerSpecificParameters::BrukerSpecificParameters(
    io::TextReader& reader, std::optional<std::string>& nextLine)
{
    parse(reader, nextLine);
}

void libjdx::jdx::BrukerSpecificParameters::parse(
    io::TextReader& reader, std::optional<std::string>& nextLine)
{
    if (!nextLine.has_value()
        || !util::isBrukerSpecificSectionStart(nextLine.value()))
    {
        throw ParseException("Illegal start of Bruker specific section: "
                             + nextLine.value_or(""));
    }
    if (reader.eof())
    {
        throw ParseException(
            "Illegal body of Bruker specific section. No content in: "
            + nextLine.value_or(""));
    }

    m_name = util::stripLineComment(nextLine.value(), false, true)
                 .second.value_or("");
    nextLine = reader.readLine();
    if (!isDashedLine(nextLine))
    {
        throw ParseException(
            "Illegal body of Bruker specific section. No dash line after: "
            + m_name);
    }
    nextLine = reader.eof() ? std::optional<std::string>{} : reader.readLine();

    parseLdrs(reader, nextLine);
}

bool libjdx::jdx::BrukerSpecificParameters::isDashedLine(
    std::optional<std::string>& nextLine)
{
    if (!nextLine.has_value())
    {
        return false;
    }
    auto [content, comment]
        = util::stripLineComment(nextLine.value(), true, true);
    if (content.empty() && comment.has_value())
    {
        return comment.value().rfind("-----", 0) == 0;
    }
    return false;
}

void libjdx::jdx::BrukerSpecificParameters::parseLdrs(
    io::TextReader& reader, std::optional<std::string>& nextLine)
{
    while (nextLine.has_value()
           && !util::isBrukerSpecificSectionStart(nextLine.value())
           && !util::isBrukerSpecificSectionEnd(nextLine.value()))
    {
        // TODO: skip other leading comments?
        auto [label, value] = util::parseLdrStart(nextLine.value());
        nextLine = parseStringValue(value, reader);
        m_content.emplace_back(label, value);
    }

    if (!nextLine.has_value()
        || (!util::isBrukerSpecificSectionStart(nextLine.value())
            && !util::isBrukerSpecificSectionEnd(nextLine.value())))
    {
        throw ParseException("Unexpected end of Bruker specific section:"
                             + std::string{s_sectionEndText});
    }

    if (util::isBrukerSpecificSectionEnd(nextLine.value()))
    {
        // skip dashes after section end marker
        nextLine
            = reader.eof() ? std::optional<std::string>{} : reader.readLine();
        if (!isDashedLine(nextLine))
        {
            throw ParseException("Illegal end of Bruker specific "
                                 "section. No dash line after: "
                                 + std::string{s_sectionEndText});
        }
        nextLine
            = reader.eof() ? std::optional<std::string>{} : reader.readLine();
    }
}

std::string libjdx::jdx::BrukerSpecificParameters::getName() const
{
    return m_name;
}

std::vector<libjdx::jdx::StringLdr>
libjdx::jdx::BrukerSpecificParameters::getContent() const
{
    return m_content;
}
