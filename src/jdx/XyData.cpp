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
#include "jdx/XyData.hpp"
#include "jdx/ParseException.hpp"
#include "jdx/XyBase.hpp"

sciformats::jdx::XyData::XyData(const std::string& label,
    const std::string& variableList, const std::vector<StringLdr>& ldrs,
    io::TextReader& reader, std::optional<std::string>& nextLine)
    : XyBase(label, variableList, ldrs, s_xyDataLabel,
        std::vector<std::string>{
            s_xyDataVariableLists.begin(), s_xyDataVariableLists.end()},
        reader, nextLine)
{
}

std::vector<std::pair<double, double>> sciformats::jdx::XyData::getData()
{
    auto varList = getVariableList();
    if (varList == s_ooQuirkVarList)
    {
        // Ocean Optics quirk
        return XyBase::getXYXYData();
    }
    if (std::any_of(s_xyDataVariableLists.begin(),
            s_xyDataVariableLists.end() - 1,
            [&varList](const std::string& s) { return s == varList; }))
    {
        return XyBase::getXppYYData();
    }
    throw ParseException("Unsupported variable list for XYDATA: " + varList);
}
