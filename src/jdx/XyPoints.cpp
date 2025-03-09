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
#include "jdx/XyPoints.hpp"
#include "jdx/ParseException.hpp"
#include "jdx/XyBase.hpp"

libjdx::jdx::XyPoints::XyPoints(const std::string& label,
    const std::string& variableList, const std::vector<StringLdr>& ldrs,
    io::TextReader& reader, std::optional<std::string>& nextLine)
    : XyBase(label, variableList, ldrs, s_xyPointsLabel,
        std::vector<std::string>{
            s_xyPointsVariableLists.begin(), s_xyPointsVariableLists.end()},
        reader, nextLine)
{
}

std::vector<std::pair<double, double>> libjdx::jdx::XyPoints::getData() const
{
    auto varList = getVariableList();
    if (std::any_of(s_xyPointsVariableLists.begin(),
            s_xyPointsVariableLists.end(),
            [&varList](const std::string& s) { return s == varList; }))
    {
        return XyBase::getXYXYData();
    }
    throw ParseException("Unsupported variable list for XYPOINTS: " + varList);
}
