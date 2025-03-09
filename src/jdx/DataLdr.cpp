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
#include "jdx/DataLdr.hpp"
#include "jdx/ParseException.hpp"
#include "util/LdrUtils.hpp"
#include "util/StringUtils.hpp"

sciformats::jdx::DataLdr::DataLdr(
    std::string label, std::string variableList, io::TextReader& reader)
    : Ldr{std::move(label)}
    , m_variableList{std::move(variableList)}
    , m_reader{reader}
    , m_dataPos{reader.tellg()}
{
}

const std::string& sciformats::jdx::DataLdr::getVariableList() const
{
    return m_variableList;
}

sciformats::io::TextReader& sciformats::jdx::DataLdr::getReader() const
{
    return m_reader;
}

void sciformats::jdx::DataLdr::validateInput(const std::string& label,
    const std::string& variableList, const std::string& expectedLabel,
    const std::vector<std::string>& expectedVariableLists)
{
    if (label != expectedLabel)
    {
        throw ParseException("Illegal label at " + expectedLabel
                             + " start encountered: " + label);
    }
    if (std::none_of(expectedVariableLists.begin(), expectedVariableLists.end(),
            [&variableList](const std::string& expectedVariableList) {
                return variableList == expectedVariableList;
            }))
    {
        throw ParseException("Illegal variable list for " + label
                             + " encountered: " + variableList);
    }
}
