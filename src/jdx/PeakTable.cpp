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
#include "jdx/PeakTable.hpp"
#include "util/LdrUtils.hpp"
#include "util/PeakTableParser.hpp"

#include <algorithm>
#include <tuple>

libjdx::jdx::PeakTable::PeakTable(const std::string& label,
    std::string variableList, io::TextReader& reader,
    std::optional<std::string>& nextLine)
    : TabularData(label, std::move(variableList), reader)
{
    validateInput(getLabel(), getVariableList(), s_peakTableLabel,
        std::vector<std::string>{std::begin(s_peakTableVariableLists),
            std::end(s_peakTableVariableLists)});
    util::skipToNextLdr(reader, nextLine, true);
}

std::vector<libjdx::jdx::Peak> libjdx::jdx::PeakTable::getData() const
{
    util::PeakTableParser parser{getReader(), getVariableList()};
    return TabularData::getData<util::PeakTableParser, Peak>(parser);
}
