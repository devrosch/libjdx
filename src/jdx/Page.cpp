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
#include "jdx/Page.hpp"
#include "jdx/ParseException.hpp"
#include "util/DataParser.hpp"
#include "util/LdrUtils.hpp"
#include "util/PeakTableParser.hpp"
#include "util/StringUtils.hpp"

libjdx::jdx::Page::Page(std::string& label, std::string pageVar,
    const std::vector<NTuplesAttributes>& nTuplesAttributes,
    const std::vector<StringLdr>& blockLdrs, io::TextReader& reader,
    std::optional<std::string>& nextLine)
    : m_pageVariables{std::move(pageVar)}
{
    validateInput(label);
    parse(nTuplesAttributes, blockLdrs, reader, nextLine);
}

void libjdx::jdx::Page::validateInput(const std::string& label)
{
    if (label != s_label)
    {
        throw ParseException("Illegal label at " + std::string{s_label}
                             + " start encountered: " + label);
    }
}

const std::string& libjdx::jdx::Page::getPageVariables() const
{
    return m_pageVariables;
}

const std::vector<libjdx::jdx::StringLdr>&
libjdx::jdx::Page::getPageLdrs() const
{
    return m_pageLdrs;
}

const std::optional<libjdx::jdx::DataTable>&
libjdx::jdx::Page::getDataTable() const
{
    return m_dataTable;
}

void libjdx::jdx::Page::parse(
    const std::vector<NTuplesAttributes>& nTuplesAttributes,
    const std::vector<StringLdr>& blockLdrs, io::TextReader& reader,
    std::optional<std::string>& nextLine)
{
    // skip potential comment lines
    util::skipPureComments(reader, nextLine, false);
    m_pageLdrs = parsePageLdrs(reader, nextLine);
    if (!nextLine.has_value() || !util::isLdrStart(nextLine.value()))
    {
        throw ParseException(
            "Unexpected content found while parsing NTUPLES PAGE: "
            + nextLine.value_or("<end of file>"));
    }

    auto [label, value] = util::parseLdrStart(nextLine.value());
    if (label == "PAGE" || label == "ENDNTUPLES" || label == "END")
    {
        // end of page, page is empty
        return;
    }
    if (label != "DATATABLE")
    {
        throw ParseException(
            "Unexpected content found while parsing NTUPLES PAGE: "
            + nextLine.value());
    }

    auto [dataTableVarList, plotDesc] = parseDataTableVars(value);
    m_dataTable.emplace(DataTable(label, dataTableVarList, plotDesc, blockLdrs,
        nTuplesAttributes, m_pageLdrs, reader, nextLine));
}

std::vector<libjdx::jdx::StringLdr> libjdx::jdx::Page::parsePageLdrs(
    io::TextReader& reader, std::optional<std::string>& nextLine)
{
    std::vector<StringLdr> pageLdrs;
    while (nextLine.has_value())
    {
        auto [label, value] = util::parseLdrStart(nextLine.value());
        if (label == "PAGE" || label == "ENDNTUPLES" || label == "END"
            || label == "DATATABLE")
        {
            // end of page or start of DATA TABLE
            break;
        }
        // LDR is a regular LDR
        nextLine = parseStringValue(value, reader);
        pageLdrs.emplace_back(label, value);
    }
    return pageLdrs;
}

std::pair<std::string, std::optional<std::string>>
libjdx::jdx::Page::parseDataTableVars(const std::string& rawPageVars)
{
    auto rawPageVarsTrimmed = util::stripLineComment(rawPageVars, true).first;
    if (rawPageVarsTrimmed.empty())
    {
        // empty
        throw ParseException(
            "Missing variable list in DATA TABLE: " + rawPageVars);
    }
    // C++ does not support lookbehind syntax R"((?<=\))\s*,\s*)", so instead
    // use non capturing group for ")" and split at capturing group
    auto segments
        = util::split(rawPageVarsTrimmed, R"((?:\))(\s*,\s*))", true, 1);
    if (segments.empty() || segments.size() > 2)
    {
        throw ParseException(
            "Unexpected content found at DATA TABLE start: " + rawPageVars);
    }

    if (segments.size() == 1)
    {
        auto varList = util::stripLineComment(segments.at(0), true).first;
        return {varList, std::nullopt};
    }
    // plot descriptor is present
    auto varList = segments.at(0);
    util::trim(varList);
    auto plotDesc = util::stripLineComment(segments.at(1), true).first;
    return {varList, plotDesc};
}
