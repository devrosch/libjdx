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
#include "jdx/Block.hpp"
#include "util/LdrUtils.hpp"
#include "util/StringUtils.hpp"

libjdx::jdx::Block::Block(std::unique_ptr<io::TextReader> readerPtr)
    : m_readerPtr{std::move(readerPtr)}
    , m_reader{*m_readerPtr}
{
    auto firstLine = m_reader.readLine();
    auto titleFirstLine = parseFirstLine(firstLine);
    std::optional<std::string> nextLine;
    parseInput(titleFirstLine, nextLine);
}

libjdx::jdx::Block::Block(io::TextReader& reader)
    : m_readerPtr{nullptr}
    , m_reader{reader}
{
    auto firstLine = reader.readLine();
    auto titleFirstLine = parseFirstLine(firstLine);
    std::optional<std::string> nextLine;
    parseInput(titleFirstLine, nextLine);
}

libjdx::jdx::Block::Block(const std::string& title, io::TextReader& reader,
    std::optional<std::string>& nextLine)
    : m_readerPtr{nullptr}
    , m_reader{reader}
{
    parseInput(title, nextLine);
}

std::optional<const libjdx::jdx::StringLdr> libjdx::jdx::Block::getLdr(
    const std::string& label) const
{
    return util::findLdr(m_ldrs, label);
}

const std::vector<libjdx::jdx::StringLdr>& libjdx::jdx::Block::getLdrs() const
{
    return m_ldrs;
}

const std::vector<libjdx::jdx::Block>& libjdx::jdx::Block::getBlocks() const
{
    return m_blocks;
}

const std::vector<std::string>& libjdx::jdx::Block::getLdrComments() const
{
    return m_ldrComments;
}

const std::optional<libjdx::jdx::XyData>& libjdx::jdx::Block::getXyData() const
{
    return m_xyData;
}

const std::optional<libjdx::jdx::RaData>& libjdx::jdx::Block::getRaData() const
{
    return m_raData;
}

const std::optional<libjdx::jdx::XyPoints>&
libjdx::jdx::Block::getXyPoints() const
{
    return m_xyPoints;
}

const std::optional<libjdx::jdx::PeakTable>&
libjdx::jdx::Block::getPeakTable() const
{
    return m_peakTable;
}

const std::optional<libjdx::jdx::PeakAssignments>&
libjdx::jdx::Block::getPeakAssignments() const
{
    return m_peakAssignments;
}

const std::optional<libjdx::jdx::NTuples>&
libjdx::jdx::Block::getNTuples() const
{
    return m_nTuples;
}

const std::optional<libjdx::jdx::AuditTrail>&
libjdx::jdx::Block::getAuditTrail() const
{
    return m_auditTrail;
}

const std::vector<libjdx::jdx::BrukerSpecificParameters>&
libjdx::jdx::Block::getBrukerSpecificParameters() const
{
    return m_brukerSpecificParameters;
}

const std::vector<libjdx::jdx::BrukerRelaxSection>&
libjdx::jdx::Block::getBrukerRelaxSections() const
{
    return m_brukerRelaxSections;
}

std::string libjdx::jdx::Block::parseFirstLine(const std::string& firstLine)
{
    if (!util::isLdrStart(firstLine))
    {
        throw BlockParseException("Malformed LDR start: " + firstLine);
    }
    auto [label, value] = util::parseLdrStart(firstLine);
    if (label != s_blockStartLabel)
    {
        throw BlockParseException(
            "Malformed Block start, wrong label: " + firstLine);
    }
    return value;
}

// NOLINTBEGIN(readability-function-cognitive-complexity)
void libjdx::jdx::Block::parseInput(
    const std::string& titleValue, std::optional<std::string>& nextLine)
{
    std::string title = titleValue;
    nextLine = parseStringValue(title, m_reader);
    m_ldrs.emplace_back(s_blockStartLabel, title);

    while (nextLine.has_value())
    {
        if (util::isPureComment(nextLine.value()))
        {
            // check for Bruker quirk
            if (util::isBrukerSpecificSectionStart(nextLine.value()))
            {
                m_brukerSpecificParameters.emplace_back(m_reader, nextLine);
                continue;
            }
            util::skipPureComments(m_reader, nextLine, true);
            continue;
        }
        // "auto [label, value] = util::parseLdrStart(nextLine.value());" cannot
        // be used as lambdas (below) cannot capture these variables
        // see:
        // https://stackoverflow.com/questions/46114214/lambda-implicit-capture-fails-with-variable-declared-from-structured-binding
        std::string label;
        std::string value;
        std::tie(label, value) = util::parseLdrStart(nextLine.value());
        if (label.empty())
        {
            // LDR start is an LDR comment "##="
            nextLine = parseStringValue(value, m_reader);
            m_ldrComments.push_back(value);
        }
        else if ("END" == label)
        {
            // end of block
            break;
        }
        else if (s_blockStartLabel == label)
        {
            // nested block
            auto block = Block(value, m_reader, nextLine);
            m_blocks.push_back(std::move(block));
        }
        else if ("XYDATA" == label)
        {
            addLdr<XyData>(title, "XYDATA", m_xyData, [&]() {
                return XyData(label, value, m_ldrs, m_reader, nextLine);
            });
        }
        else if ("RADATA" == label)
        {
            addLdr<RaData>(title, "RADATA", m_raData, [&]() {
                return RaData(label, value, m_ldrs, m_reader, nextLine);
            });
        }
        else if ("XYPOINTS" == label)
        {
            addLdr<XyPoints>(title, "XYPOINTS", m_xyPoints, [&]() {
                return XyPoints(label, value, m_ldrs, m_reader, nextLine);
            });
        }
        else if ("PEAKTABLE" == label)
        {
            addLdr<PeakTable>(title, "PEAKTABLE", m_peakTable,
                [&]() { return PeakTable(label, value, m_reader, nextLine); });
        }
        else if ("PEAKASSIGNMENTS" == label)
        {
            addLdr<PeakAssignments>(
                title, "PEAKASSIGNMENTS", m_peakAssignments, [&]() {
                    return PeakAssignments(label, value, m_reader, nextLine);
                });
        }
        else if ("NTUPLES" == label)
        {
            addLdr<NTuples>(title, "NTUPLES", m_nTuples, [&]() {
                return NTuples(label, value, m_ldrs, m_reader, nextLine);
            });
        }
        else if ("AUDITTRAIL" == label)
        {
            addLdr<AuditTrail>(title, "AUDITTRAIL", m_auditTrail,
                [&]() { return AuditTrail(label, value, m_reader, nextLine); });
        }
        else if ("$RELAX" == label)
        {
            // RELAX section start
            auto relaxSection
                = BrukerRelaxSection(label, value, m_reader, nextLine);
            // only add non blank sections
            // section may be blank if ##$RELAX= line is immediately followed by
            // $$ Bruker specific parameters
            if (!relaxSection.getName().empty()
                || !relaxSection.getContent().empty())
            {
                m_brukerRelaxSections.push_back(std::move(relaxSection));
            }
        }
        else
        {
            // LDR is a regular LDR
            nextLine = parseStringValue(value, m_reader);
            // duplicate?
            std::optional<StringLdr> existingLdr = getLdr(label);
            if (existingLdr)
            {
                // reference implementation seems to overwrite LDR with
                // duplicate, but spec (JCAMP-DX IR 3.2) says
                // a duplicate LDR is illegal in a block
                // => accept if content is identical
                if (existingLdr.has_value()
                    && existingLdr.value().getValue() != value)
                {
                    throw BlockParseException(
                        "Multiple non-identical values found for \"" + label
                        + std::string{"\" in block: \"" + title + "\""});
                }
            }
            m_ldrs.emplace_back(label, value);
        }
    }

    if (!nextLine.has_value()
        || "END" != util::parseLdrStart(nextLine.value()).first)
    {
        throw BlockParseException("No", "END", title);
    }
    // make nextline the one following the ##END= LDR
    nextLine = m_reader.eof() ? std::nullopt
                              : std::optional<std::string>{m_reader.readLine()};
}
// NOLINTEND(readability-function-cognitive-complexity)