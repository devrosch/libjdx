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
#include "util/LdrUtils.hpp"
#include "jdx/ParseException.hpp"
#include "util/StringUtils.hpp"

#include <algorithm>
#include <regex>
#include <string>
#include <utility>

bool libjdx::jdx::util::isLdrStart(const std::string& line)
{
    static const std::regex regex{"\\s*##.*=.*"};
    // match_continuous => match at start only
    return std::regex_search(
        line, regex, std::regex_constants::match_continuous);
}

std::string libjdx::jdx::util::normalizeLdrStart(const std::string& ldr)
{
    std::string output{};
    auto it = ldr.cbegin();
    // skip leading white spaces
    for (; it != ldr.cend(); ++it)
    {
        if (!isSpace(*it))
        {
            break;
        }
    }
    // check and skip "##" marking start of LDR
    for (auto i{0}; i < 2; i++)
    {
        if (it == ldr.cend() || *it != '#')
        {
            throw ParseException(
                "Malformed LDR start, missing double hashes: " + ldr);
        }
        output += *it++;
    }
    // normalize label
    std::string label{};
    while (it != ldr.cend() && *it != '=')
    {
        label += *it++;
    }
    output.append(normalizeLdrLabel(label));
    // add remaining string content
    if (it == ldr.cend() || *it != '=')
    {
        throw ParseException("Malformed LDR start, missing equals: " + ldr);
    }
    output.append(it, ldr.end());
    return output;
}

std::string libjdx::jdx::util::normalizeLdrLabel(const std::string& label)
{
    std::string output{};
    // normalize LDR label, i.e. the string between ## and =
    auto makeUpperCase = [](const unsigned char c) { return std::toupper(c); };
    for (const char c : label)
    {
        if (c == ' ' || c == '-' || c == '/' || c == '_')
        {
            // discard
            continue;
        }
        output
            += static_cast<char>(makeUpperCase(static_cast<unsigned char>(c)));
    }
    return output;
}

std::pair<std::string, std::string> libjdx::jdx::util::parseLdrStart(
    const std::string& ldrStart)
{
    size_t posEquals = ldrStart.find('=');
    if (std::string::npos == posEquals)
    {
        throw ParseException(
            "Malformed LDR start, missing equals: " + ldrStart);
    }
    std::string label = ldrStart.substr(0, posEquals + 1);
    std::string normalizedLabel = normalizeLdrStart(label);
    if (normalizedLabel.size() < 3 || normalizedLabel.at(0) != '#'
        || normalizedLabel.at(1) != '#'
        || normalizedLabel.at(normalizedLabel.size() - 1) != '=')
    {
        throw ParseException(
            "Malformed LDR start, normalization yields illegal label: "
            + normalizedLabel);
    }
    // strip leading and trailing symbols from label
    normalizedLabel.erase(0, 2);
    normalizedLabel.erase(normalizedLabel.size() - 1);

    std::string value = ldrStart.substr(posEquals + 1);
    if (!value.empty() && value.at(0) == ' ')
    {
        // strip leading blank from value if present
        value.erase(0, 1);
    }

    return std::make_pair(normalizedLabel, value);
}

std::pair<std::string, std::optional<std::string>>
libjdx::jdx::util::stripLineComment(
    const std::string& line, bool trimContent, bool trimComment)
{
    const auto pos = line.find("$$");
    if (pos == std::string::npos)
    {
        // no comment
        if (!trimContent)
        {
            return make_pair(line, std::nullopt);
        }
        auto trimmedLine{line};
        util::trim(trimmedLine);
        return make_pair(trimmedLine, std::nullopt);
    }

    // separate comment
    auto content = line.substr(0, pos);
    auto comment = line.substr(pos + 2);
    if (trimContent)
    {
        util::trim(content);
    }
    if (trimComment)
    {
        util::trim(comment);
    }
    return std::make_pair(content, comment);
}

std::optional<const libjdx::jdx::StringLdr> libjdx::jdx::util::findLdr(
    const std::vector<StringLdr>& ldrs, const std::string& label)
{
    std::string normalizedLabel = normalizeLdrLabel(label);
    auto it = std::find_if(
        ldrs.begin(), ldrs.end(), [&normalizedLabel](const StringLdr& ldr) {
            return ldr.getLabel() == normalizedLabel;
        });

    if (it != ldrs.cend())
    {
        return *it;
    }
    return std::nullopt;
}

std::optional<std::string> libjdx::jdx::util::findLdrValue(
    const std::vector<StringLdr>& ldrs, const std::string& label)
{
    auto ldr = util::findLdr(ldrs, label);
    return ldr.has_value() ? std::optional<std::string>(ldr.value().getValue())
                           : std::optional<std::string>(std::nullopt);
}

void libjdx::jdx::util::skipToNextLdr(io::TextReader& reader,
    std::optional<std::string>& nextLine, bool forceSkipFirstLine)
{
    if (forceSkipFirstLine)
    {
        if (reader.eof())
        {
            nextLine = std::nullopt;
        }
        else
        {
            nextLine = reader.readLine();
        }
    }
    while (nextLine.has_value() && !util::isLdrStart(nextLine.value()))
    {
        if (reader.eof())
        {
            nextLine = std::nullopt;
            continue;
        }
        nextLine = reader.readLine();
    }
}

void libjdx::jdx::util::skipPureComments(io::TextReader& reader,
    std::optional<std::string>& nextLine, bool mustPrecedeLdr)
{
    while (nextLine)
    {
        if (util::isPureComment(nextLine.value()))
        {
            nextLine = reader.eof()
                           ? std::nullopt
                           : std::optional<std::string>{reader.readLine()};
            continue;
        }
        if (mustPrecedeLdr && !util::isLdrStart(nextLine.value()))
        {
            // pure $$ comment lines must be followed by LDR start
            // if not this special case, give up
            throw ParseException(
                "Unexpected content found instead of pure comment ($$): "
                + nextLine.value());
        }
        break;
    }
}

bool libjdx::jdx::util::isPureComment(const std::string& line)
{
    // only $$ comment?
    auto preCommentValue = util::stripLineComment(line, true).first;
    return preCommentValue.empty();
}

bool libjdx::jdx::util::isBrukerSpecificSectionStart(const std::string& line)
{
    return line.rfind("$$ Bruker specific parameters", 0) == 0;
}

bool libjdx::jdx::util::isBrukerSpecificSectionEnd(const std::string& line)
{
    return line.rfind("$$ End of Bruker specific parameters", 0) == 0;
}
