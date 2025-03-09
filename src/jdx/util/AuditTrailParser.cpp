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
#include "util/AuditTrailParser.hpp"
#include "jdx/AuditTrailEntry.hpp"
#include "jdx/ParseException.hpp"
#include "util/LdrUtils.hpp"
#include "util/StringUtils.hpp"

#include <algorithm>
#include <map>
#include <regex>

libjdx::jdx::util::AuditTrailParser::AuditTrailParser(
    io::TextReader& reader, std::string variableList)
    : MultilineTuplesParser(reader, std::move(variableList), s_ldrName, "\n")
{
}

std::optional<libjdx::jdx::AuditTrailEntry>
libjdx::jdx::util::AuditTrailParser::next()
{
    return TuplesParser::next<AuditTrailEntry>([this]() { return nextTuple(); },
        [this](
            const std::string& tuple) { return createAuditTrailEntry(tuple); });
}

libjdx::jdx::AuditTrailEntry
libjdx::jdx::util::AuditTrailParser::createAuditTrailEntry(
    const std::string& tuple) const
{
    // tokenize
    // token[0] is the full match so extract 1 + 7 tokens for 7 groups
    const auto tokens = extractTokens(tuple, m_regex, 1 + 7);
    const auto& varList = getVariableList();

    // error conditions {varList, {error condition, error message}}
    const std::multimap<std::string, std::tuple<bool, std::string>> errorMap{
        {s_varLists.at(0),
            {tokens.at(5) || tokens.at(6),
                std::string{"Illegal "} + s_ldrName + " entry for "
                    + s_varLists.at(0) + ": " + tuple}},
        {s_varLists.at(1),
            {!tokens.at(5) || tokens.at(6),
                std::string{"Illegal "} + s_ldrName + " entry for "
                    + s_varLists.at(1) + ": " + tuple}},
        {s_varLists.at(2),
            {!tokens.at(5) || !tokens.at(6),
                std::string{"Illegal "} + s_ldrName + " entry for "
                    + s_varLists.at(2) + ": " + tuple}},
    };

    checkForErrors(varList, errorMap, s_ldrName);

    // map
    AuditTrailEntry entry{};
    entry.number = strtol(tokens.at(1).value().data(), nullptr, 10);
    entry.when = tokens.at(2).value();
    entry.who = tokens.at(3).value();
    entry.where = tokens.at(4).value();
    entry.what = tokens.at(7).value();
    // nothing to do for varLists[0]
    if (s_varLists.at(1) == varList)
    {
        entry.version = tokens.at(5).value();
    }
    else if (s_varLists.at(2) == varList)
    {
        entry.process = tokens.at(5).value();
        entry.version = tokens.at(6).value();
    }

    return entry;
}
