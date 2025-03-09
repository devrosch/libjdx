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
#include "jdx/AuditTrail.hpp"
#include "util/AuditTrailParser.hpp"
#include "util/LdrUtils.hpp"
#include "util/StringUtils.hpp"

#include <algorithm>

libjdx::jdx::AuditTrail::AuditTrail(const std::string& label,
    std::string variableList, io::TextReader& reader,
    std::optional<std::string>& nextLine)
    : TabularData(label, std::move(variableList), reader)
{
    auto varList = getVariableList();
    util::trim(varList);
    validateInput(getLabel(), varList, s_label,
        std::vector<std::string>{
            std::begin(s_variableLists), std::end(s_variableLists)});

    // Bruker quirk: check if overruling Bruker var list is present
    m_brukerVarList = scanForBrukerVarList(nextLine);
    if (m_brukerVarList)
    {
        validateInput(getLabel(), m_brukerVarList.value(), s_label,
            std::vector<std::string>{
                std::begin(s_variableLists), std::end(s_variableLists)});
    }

    util::skipToNextLdr(reader, nextLine, false);
}

std::vector<libjdx::jdx::AuditTrailEntry>
libjdx::jdx::AuditTrail::getData() const
{
    auto variableList = m_brukerVarList.value_or(getVariableList());
    if (util::isPureComment(variableList))
    {
        // deal with variable lists that sit behind "$$"
        variableList
            = util::stripLineComment(variableList, false, true).second.value();
    }
    util::AuditTrailParser parser{getReader(), variableList};
    return TabularData::getData<util::AuditTrailParser, AuditTrailEntry>(
        parser);
}

std::optional<std::string> libjdx::jdx::AuditTrail::scanForBrukerVarList(
    std::optional<std::string>& nextLine)
{
    auto& reader = getReader();
    if (!reader.eof())
    {
        nextLine = reader.readLine();
        if (!nextLine
            || nextLine.value().rfind("$$ ##TITLE= Audit trail,", 0) != 0)
        {
            return std::nullopt;
        }
    }
    // Bruker audit trail
    while (!reader.eof())
    {
        nextLine = reader.readLine();
        if (!nextLine || !util::isPureComment(nextLine.value()))
        {
            break;
        }
        if (nextLine.value().rfind("$$ ##AUDIT TRAIL=", 0) == 0)
        {
            auto brukerAuditTrail
                = util::stripLineComment(nextLine.value(), false, true)
                      .second.value();
            auto brukerVarList = util::parseLdrStart(brukerAuditTrail).second;
            util::trim(brukerVarList);
            return brukerVarList;
        }
    }
    return std::nullopt;
}
