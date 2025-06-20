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
#include "jdx/NTuples.hpp"
#include "jdx/ParseException.hpp"
#include "util/LdrUtils.hpp"
#include "util/StringUtils.hpp"

#include <utility>

libjdx::jdx::NTuples::NTuples(const std::string& label, std::string dataForm,
    const std::vector<StringLdr>& blockLdrs, io::TextReader& reader,
    std::optional<std::string>& nextLine)
    : m_dataForm{std::move(dataForm)}
{
    validateInput(label);
    parse(blockLdrs, reader, nextLine);
}

std::string libjdx::jdx::NTuples::getDataForm() const
{
    return m_dataForm;
}

const std::vector<libjdx::jdx::StringLdr>& libjdx::jdx::NTuples::getLdrs() const
{
    return m_ldrs;
}

const std::vector<libjdx::jdx::NTuplesAttributes>&
libjdx::jdx::NTuples::getAttributes() const
{
    return m_attributes;
}

size_t libjdx::jdx::NTuples::getNumPages() const
{
    return m_pages.size();
}

const libjdx::jdx::Page& libjdx::jdx::NTuples::getPage(size_t pageIndex) const
{
    return m_pages.at(pageIndex);
}

void libjdx::jdx::NTuples::validateInput(const std::string& label)
{
    if (label != s_label)
    {
        throw ParseException("Illegal label at " + std::string{s_label}
                             + " start encountered: " + label);
    }
}

void libjdx::jdx::NTuples::parse(const std::vector<StringLdr>& blockLdrs,
    io::TextReader& reader, std::optional<std::string>& nextLine)
{
    // skip potential comment lines
    nextLine = reader.eof() ? std::nullopt
                            : std::optional<std::string>{reader.readLine()};
    util::skipPureComments(reader, nextLine, true);
    // parse PAGE parameters
    m_attributes = parseAttributes(reader, nextLine);
    // parse pages
    while (nextLine.has_value() && util::isLdrStart(nextLine.value()))
    {
        auto [label, pageVar] = util::parseLdrStart(nextLine.value());
        pageVar = util::stripLineComment(pageVar, true).first;
        if (label == "ENDNTUPLES")
        {
            // ##END NTUPLES is described as optional in JCAMP6_2b Draft
            // but is required for indicating the NTUPLES end

            // skip ##END NTUPLES
            nextLine = reader.eof()
                           ? std::nullopt
                           : std::optional<std::string>{reader.readLine()};
            return;
        }
        if (label != "PAGE")
        {
            throw ParseException("Unexpected content found in NTUPLES record: "
                                 + nextLine.value());
        }
        nextLine = reader.eof() ? std::nullopt
                                : std::optional<std::string>{reader.readLine()};
        auto page
            = Page(label, pageVar, m_attributes, blockLdrs, reader, nextLine);
        m_pages.push_back(std::move(page));
    }
    if (!nextLine.has_value())
    {
        throw ParseException("Unexpected end of NTUPLES record: " + m_dataForm);
    }
}

std::vector<libjdx::jdx::NTuplesAttributes>
libjdx::jdx::NTuples::parseAttributes(
    io::TextReader& reader, std::optional<std::string>& nextLine)
{
    m_ldrs = readLdrs(nextLine, reader);
    auto attrMap = splitValues(m_ldrs);
    auto standardAttrMap = extractStandardAttributes(attrMap);

    auto attrNames = findValue("VARNAME", standardAttrMap);
    if (!attrNames)
    {
        // VARNAMEs are required by the spec
        throw ParseException(
            "No \"VAR_NAME\" LDR found in NTUPLES: " + m_dataForm);
    }
    if (!attrNames.value().empty())
    {
        // check if last VAR_NAME is blank, i.e., there is a trailing comma
        // if so, remove, thus ignore column in subsequent processing
        // required to sucessfully process test data set
        auto lastVarName = attrNames.value().back();
        util::trim(lastVarName);
        if (lastVarName.empty())
        {
            attrNames.value().pop_back();
        }
    }
    auto numAttrs = attrNames.value().size();

    std::vector<NTuplesAttributes> output;
    for (size_t i = 0; i < numAttrs; ++i)
    {
        auto ntv = map(standardAttrMap, attrMap, i);
        output.push_back(ntv);
    }
    return output;
}

std::vector<libjdx::jdx::StringLdr> libjdx::jdx::NTuples::readLdrs(
    std::optional<std::string>& firstLdrStart, io::TextReader& reader)
{
    std::optional<std::string>& nextLine = firstLdrStart;
    std::vector<StringLdr> output;
    while (nextLine)
    {
        auto [title, value] = util::parseLdrStart(nextLine.value());
        if (title == "PAGE" || title == "ENDNTUPLES" || title == "END")
        {
            // all NTUPLES LDRs read
            break;
        }
        nextLine = parseStringValue(value, reader);
        output.emplace_back(title, value);
    }
    return output;
}

std::map<std::string, std::vector<std::string>>
libjdx::jdx::NTuples::splitValues(const std::vector<StringLdr>& ldrs)
{
    std::map<std::string, std::vector<std::string>> output;
    for (const auto& ldr : ldrs)
    {
        auto valueString = util::stripLineComment(ldr.getValue(), true).first;
        auto values = util::split(valueString, ",", true);
        auto inserted = output.emplace(ldr.getLabel(), values).second;
        if (!inserted)
        {
            throw ParseException(
                "Duplicate LDR found in NTUPLE: " + ldr.getLabel());
        }
    }
    return output;
}

std::map<std::string, std::vector<std::string>>
libjdx::jdx::NTuples::extractStandardAttributes(
    std::map<std::string, std::vector<std::string>>& attributes)
{
    // see:
    // https://stackoverflow.com/questions/180516/how-to-filter-items-from-a-stdmap
    auto matches = [](const std::string& attr) {
        return std::find(std::begin(s_standardAttrNames),
                   std::end(s_standardAttrNames), attr)
               != std::end(s_standardAttrNames);
    };
    std::map<std::string, std::vector<std::string>> standardAttrs;
    // remove standard attributes
    for (auto it = attributes.begin(); it != attributes.end();)
    {
        if (matches((*it).first))
        {
            standardAttrs.insert(*it);
            attributes.erase(it++);
        }
        else
        {
            ++it;
        }
    }
    return standardAttrs;
}

libjdx::jdx::NTuplesAttributes libjdx::jdx::NTuples::map(
    const std::map<std::string, std::vector<std::string>>& standardAttributes,
    const std::map<std::string, std::vector<std::string>>& additionalAttributes,
    size_t valueColumnIndex)
{
    auto pickColumnValue
        = [valueColumnIndex](
              const std::optional<std::vector<std::string>>& values) {
              if (values.value().size() <= valueColumnIndex)
              {
                  return std::optional<std::string>{std::nullopt};
              }
              auto value = values.value().at(valueColumnIndex);
              util::trim(value);
              return std::optional<std::string>{
                  values.value().at(valueColumnIndex)};
          };

    auto findColumnValue
        = [&pickColumnValue, &standardAttributes = std::as_const(
                                 standardAttributes)](const std::string& key) {
              auto values = findValue(key, standardAttributes);
              if (!values)
              {
                  return std::optional<std::string>{std::nullopt};
              }
              return pickColumnValue(values);
          };

    auto varNameString = findColumnValue("VARNAME");
    if (!varNameString)
    {
        // VARNAMEs are required by the spec
        throw ParseException(
            R"(No "VAR_NAME" LDR found in NTUPLES ")" + m_dataForm
            + "\" column: " + std::to_string(valueColumnIndex));
    }
    auto symbolString = findColumnValue("SYMBOL");
    auto varTypeString = findColumnValue("VARTYPE");
    auto varFormString = findColumnValue("VARFORM");
    auto varDimString = findColumnValue("VARDIM");
    auto unitsString = findColumnValue("UNITS");
    auto firstString = findColumnValue("FIRST");
    auto lastString = findColumnValue("LAST");
    auto minString = findColumnValue("MIN");
    auto maxString = findColumnValue("MAX");
    auto factorString = findColumnValue("FACTOR");

    // TODO: make stol/stod more reliable (new util function) and use across
    // project
    if (!varNameString)
    {
        throw ParseException("VAR_NAME missing in NTUPLES column: "
                             + std::to_string(valueColumnIndex));
    }
    if (!symbolString)
    {
        throw ParseException("SYMBOL missing in NTUPLES column: "
                             + std::to_string(valueColumnIndex));
    }
    auto varName = varNameString.value();
    auto symbol = symbolString.value();
    auto& varType = varTypeString;
    auto& varForm = varFormString;

    auto varDim = varDimString && !varDimString.value().empty()
                      ? std::optional<uint64_t>(std::stol(varDimString.value()))
                      : std::nullopt;
    auto& units = unitsString;
    auto first = firstString && !firstString.value().empty()
                     ? std::optional<double>(std::stod(firstString.value()))
                     : std::nullopt;
    auto last = lastString && !lastString.value().empty()
                    ? std::optional<double>(std::stod(lastString.value()))
                    : std::nullopt;
    auto min = minString && !minString.value().empty()
                   ? std::optional<double>(std::stod(minString.value()))
                   : std::nullopt;
    auto max = maxString && !maxString.value().empty()
                   ? std::optional<double>(std::stod(maxString.value()))
                   : std::nullopt;
    auto factor = factorString && !factorString.value().empty()
                      ? std::optional<double>(std::stod(factorString.value()))
                      : std::nullopt;

    std::vector<StringLdr> additionalAttrsInColum;
    for (const auto& var : additionalAttributes)
    {
        auto value = pickColumnValue(var.second);
        if (value)
        {
            additionalAttrsInColum.emplace_back(var.first, value.value());
        }
    }

    NTuplesAttributes nTupleAttrs{
        varName,
        symbol,
        varType,
        varForm,
        varDim,
        units,
        first,
        last,
        min,
        max,
        factor,
        additionalAttrsInColum,
    };

    return nTupleAttrs;
}

std::optional<std::vector<std::string>> libjdx::jdx::NTuples::findValue(
    const std::string& key,
    const std::map<std::string, std::vector<std::string>>& map)
{
    const auto it = map.find(key);
    if (it != map.end())
    {
        return std::optional<std::vector<std::string>>{(*it).second};
    }
    return std::optional<std::vector<std::string>>{std::nullopt};
}
