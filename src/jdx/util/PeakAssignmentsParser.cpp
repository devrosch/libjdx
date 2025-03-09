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
#include "util/PeakAssignmentsParser.hpp"
#include "jdx/ParseException.hpp"
#include "jdx/PeakAssignment.hpp"
#include "util/LdrUtils.hpp"
#include "util/StringUtils.hpp"

#include <algorithm>
#include <regex>

libjdx::jdx::util::PeakAssignmentsParser::PeakAssignmentsParser(
    io::TextReader& reader, std::string variableList)
    : MultilineTuplesParser(reader, std::move(variableList), s_ldrName, " ")
{
}

std::optional<libjdx::jdx::PeakAssignment>
libjdx::jdx::util::PeakAssignmentsParser::next()
{
    return TuplesParser::next<PeakAssignment>([this]() { return nextTuple(); },
        [this](
            const std::string& tuple) { return createPeakAssignment(tuple); });
}

libjdx::jdx::PeakAssignment
libjdx::jdx::util::PeakAssignmentsParser::createPeakAssignment(
    const std::string& tuple) const
{
    // tokenize
    // token[0] is the full match so extract 1 + 5 tokens for 5 groups
    const auto tokens = extractTokens(tuple, m_regex, 1 + 5);
    const auto& varList = getVariableList();

    // error conditions {varList, {error condition, error message}}
    const std::multimap<std::string, std::tuple<bool, std::string>> errorMap{
        {s_varLists.at(0),
            {tokens.at(3) || tokens.at(4),
                std::string{"Illegal "} + s_ldrName + " entry for "
                    + s_varLists.at(0) + ": " + tuple}},
        {s_varLists.at(1),
            {tokens.at(4).has_value(), std::string{"Illegal "} + s_ldrName
                                           + " entry for " + s_varLists.at(1)
                                           + ": " + tuple}},
        {s_varLists.at(1),
            {tokens.at(2) && !tokens.at(3),
                std::string{"Ambiguous "} + s_ldrName + " entry for "
                    + s_varLists.at(1) + ": " + tuple}},
        {s_varLists.at(2),
            {tokens.at(4).has_value(), std::string{"Illegal "} + s_ldrName
                                           + " entry for " + s_varLists.at(2)
                                           + ": " + tuple}},
        {s_varLists.at(2),
            {tokens.at(2) && !tokens.at(3),
                std::string{"Ambiguous "} + s_ldrName + " entry for "
                    + s_varLists.at(2) + ": " + tuple}},
        {s_varLists.at(3),
            {!(tokens.at(2) && tokens.at(3) && tokens.at(4))
                    && (tokens.at(2) || tokens.at(3) || tokens.at(4)),
                std::string{"Ambiguous "} + s_ldrName + " entry for "
                    + s_varLists.at(2) + ": " + tuple}},
    };

    checkForErrors(varList, errorMap, s_ldrName);

    // map
    PeakAssignment peakAssignment{};
    peakAssignment.x = parseDoubleToken(tokens.at(1));
    peakAssignment.a = tokens.at(5).value();
    if (s_varLists.at(0) == varList && tokens.at(2))
    {
        // 3 tokens
        peakAssignment.y = parseDoubleToken(tokens.at(2));
    }
    else if (s_varLists.at(1) == varList && tokens.at(2) && tokens.at(3))
    {
        // 4 tokens
        peakAssignment.y = parseDoubleToken(tokens.at(2));
        peakAssignment.w = parseDoubleToken(tokens.at(3));
    }
    else if (s_varLists.at(2) == varList && tokens.at(2) && tokens.at(3))
    {
        // 4 tokens
        peakAssignment.y = parseDoubleToken(tokens.at(2));
        peakAssignment.m = tokens.at(3);
    }
    else if (s_varLists.at(3) == varList && tokens.at(2) && tokens.at(3)
             && tokens.at(4))
    {
        // 5 tokens
        peakAssignment.y = parseDoubleToken(tokens.at(2));
        peakAssignment.m = tokens.at(3);
        peakAssignment.w = parseDoubleToken(tokens.at(4));
    }

    return peakAssignment;
}
