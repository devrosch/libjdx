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
#include "jdx/api/JdxConverter.hpp"
#include "api/Point2D.hpp"
#include "jdx/JdxParser.hpp"
#include "util/StringUtils.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

sciformats::jdx::api::JdxConverter::JdxConverter(const std::string& path)
    : m_rootBlock{std::make_unique<sciformats::jdx::Block>(
        sciformats::jdx::JdxParser::parse(path))}
{
}

sciformats::api::Node sciformats::jdx::api::JdxConverter::read(
    const std::string& path)
{
    std::cout << "C++: JdxConverter.read(): " << path << '\n';
    std::vector<size_t> nodeIndices = convertPathToNodeIndices(path);
    auto node = retrieveNode(nodeIndices);
    return node;
}

std::vector<size_t>
sciformats::jdx::api::JdxConverter::convertPathToNodeIndices(
    const std::string& path)
{
    auto pathSegments = util::split(path, "/", true);
    if (!pathSegments.at(0).empty() || pathSegments.size() < 2)
    {
        throw std::invalid_argument("Illegal path for reading node: " + path);
    }

    std::vector<size_t> nodeIndices{};
    if (!(pathSegments.size() == 2 && pathSegments.at(1).empty()))
    {
        // path not only "/"
        for (size_t i = 1; i < pathSegments.size(); i++)
        {
            auto pathIndex = std::stoul(pathSegments.at(i));
            nodeIndices.push_back(pathIndex);
        }
    }

    return nodeIndices;
}

sciformats::api::Node sciformats::jdx::api::JdxConverter::retrieveNode(
    const std::vector<size_t>& nodeIndices)
{
    auto raiseIllegalPathError = [](size_t nodeIndex, const Block* block) {
        throw std::invalid_argument(
            "Illegal path for reading node. Block: "
            + block->getLdr("TITLE").value().getLabel()
            + ", child index: " + std::to_string(nodeIndex));
    };

    const Block* block = &(*m_rootBlock);
    size_t iterationIndex = 0;
    for (const auto nodeIndex : nodeIndices)
    {
        const size_t brukerRelaxStartIndex = 0;
        const size_t brukerRelaxEndIndexExclusive
            = brukerRelaxStartIndex + block->getBrukerRelaxSections().size();
        const size_t brukerParamsStartIndex = brukerRelaxEndIndexExclusive;
        const size_t brukerParamsEndIndexExclusive
            = brukerParamsStartIndex
              + block->getBrukerSpecificParameters().size();
        const size_t nTuplesIndex = brukerParamsEndIndexExclusive;
        const size_t auditTrailIndex
            = block->getNTuples().has_value() ? nTuplesIndex + 1 : nTuplesIndex;
        const size_t childBlocksStartIndex = block->getAuditTrail().has_value()
                                                 ? auditTrailIndex + 1
                                                 : auditTrailIndex;

        // nodeIndex >= brukerRelaxStartIndex, always true
        if (nodeIndex < brukerRelaxEndIndexExclusive
            && !block->getBrukerRelaxSections().empty())
        {
            // Bruker ##$RELAX section
            if (iterationIndex < nodeIndices.size() - 1)
            {
                // not a leaf node
                raiseIllegalPathError(nodeIndex, block);
            }
            return mapBrukerRelaxSection(block->getBrukerRelaxSections().at(
                nodeIndex - brukerRelaxStartIndex));
        }
        if (nodeIndex >= brukerParamsStartIndex
            && nodeIndex < brukerParamsEndIndexExclusive
            && !block->getBrukerSpecificParameters().empty())
        {
            // $$ Bruker specific parameters section
            if (iterationIndex < nodeIndices.size() - 1)
            {
                // not a leaf node
                raiseIllegalPathError(nodeIndex, block);
            }
            return mapBrukerSpecificParameters(
                block->getBrukerSpecificParameters().at(
                    nodeIndex - brukerParamsStartIndex));
        }
        if (nodeIndex == nTuplesIndex && block->getNTuples().has_value())
        {
            // consider NTUPLES LDR as child node
            auto startIt = nodeIndices.cbegin() + iterationIndex;
            auto nTuplesIndices
                = std::vector<size_t>{++startIt, nodeIndices.cend()};
            return mapNTuples(block->getNTuples().value(), nTuplesIndices,
                isPeakData(*block));
        }
        if (nodeIndex == auditTrailIndex && block->getAuditTrail().has_value())
        {
            // consider AUDIT TRAIL LDR as child node
            if (iterationIndex < nodeIndices.size() - 1)
            {
                // not a leaf node
                raiseIllegalPathError(nodeIndex, block);
            }
            return mapAuditTrail(block->getAuditTrail().value());
        }
        const auto& childBlock
            = block->getBlocks().at(nodeIndex - childBlocksStartIndex);
        block = &childBlock;
        ++iterationIndex;
    }
    return mapBlock(*block, isPeakData(*block));
}

sciformats::api::Node sciformats::jdx::api::JdxConverter::mapBlock(
    const Block& block, bool isPeakData)
{
    auto name = block.getLdr("TITLE").value().getValue();

    std::vector<sciformats::api::KeyValueParam> parameters{};
    for (auto const& ldr : block.getLdrs())
    {
        parameters.push_back({ldr.getLabel(), ldr.getValue()});
    }

    auto data = mapData(block);

    auto peakTable = sciformats::api::Table{};
    if (block.getPeakAssignments().has_value())
    {
        peakTable = mapPeakAssignments(block.getPeakAssignments().value());
    }
    else if (block.getPeakTable().has_value())
    {
        peakTable = mapPeakTable(block.getPeakTable().value());

        if (isPeakData && data.empty())
        {
            // for MS, map peak table as data if no other data is present
            data = mapPeakTableAsData(block.getPeakTable().value());
        }
    }

    std::vector<std::string> childNodeNames{};
    for (auto const& brukerRelaxSection : block.getBrukerRelaxSections())
    {
        auto childNodeName = brukerRelaxSection.getName();
        childNodeNames.push_back(childNodeName);
    }
    for (auto const& brukerSpecificSection :
        block.getBrukerSpecificParameters())
    {
        auto childNodeName = brukerSpecificSection.getName();
        childNodeNames.push_back(childNodeName);
    }
    if (block.getNTuples().has_value())
    {
        // consider NTUPLES LDR as child node
        auto nTuplesName = block.getNTuples().value().getDataForm();
        childNodeNames.push_back(nTuplesName);
    }
    if (block.getAuditTrail().has_value())
    {
        // consider AUDIT TRAIL LDR as child node
        auto auditTrailLabel = block.getAuditTrail().value().getLabel();
        childNodeNames.push_back(auditTrailLabel);
    }
    for (auto const& childBlock : block.getBlocks())
    {
        auto childNodeName = childBlock.getLdr("TITLE").value().getValue();
        childNodeNames.push_back(childNodeName);
    }

    auto metadata = mapMetadata(block);

    return {name, parameters, data, metadata, peakTable, childNodeNames};
}

bool sciformats::jdx::api::JdxConverter::isPeakData(const Block& block)
{
    auto dataType
        = block.getLdr("DATATYPE").value_or(StringLdr{"", ""}).getValue();
    util::toLower(dataType);
    return dataType == "mass spectrum";
}

sciformats::api::Node sciformats::jdx::api::JdxConverter::mapBrukerRelaxSection(
    const BrukerRelaxSection& section)
{
    std::vector<sciformats::api::KeyValueParam> parameters{};
    parameters.push_back({"", section.getContent()});
    return {
        section.getName(),
        parameters,
        std::vector<sciformats::api::Point2D>{},
        std::map<std::string, std::string>{},
        sciformats::api::Table{},
        std::vector<std::string>{},
    };
}

sciformats::api::Node
sciformats::jdx::api::JdxConverter::mapBrukerSpecificParameters(
    const BrukerSpecificParameters& section)
{
    std::vector<sciformats::api::KeyValueParam> parameters{};
    for (const auto& ldr : section.getContent())
    {
        parameters.push_back({ldr.getLabel(), ldr.getValue()});
    }
    return {
        section.getName(),
        parameters,
        std::vector<sciformats::api::Point2D>{},
        std::map<std::string, std::string>{},
        sciformats::api::Table{},
        std::vector<std::string>{},
    };
}

sciformats::api::Node sciformats::jdx::api::JdxConverter::mapNTuples(
    const NTuples& nTuples, const std::vector<size_t>& nodeIndices,
    bool isPeakData)
{
    if (nodeIndices.empty())
    {
        // map NTUPLES record
        auto name = nTuples.getDataForm();

        std::vector<sciformats::api::KeyValueParam> parameters{};
        for (const auto& ldr : nTuples.getLdrs())
        {
            parameters.push_back({ldr.getLabel(), ldr.getValue()});
        }

        std::vector<sciformats::api::Point2D> data{};

        auto peakTable = sciformats::api::Table{};

        std::vector<std::string> childNodeNames{};
        for (size_t pageIndex = 0; pageIndex < nTuples.getNumPages();
             ++pageIndex)
        {
            const auto& page = nTuples.getPage(pageIndex);
            childNodeNames.push_back(mapNTuplesPageName(page));
        }

        auto metadata = std::map<std::string, std::string>{};

        return {name, parameters, data, metadata, peakTable, childNodeNames};
    }

    if (nodeIndices.size() > 1 || nodeIndices.at(0) > nTuples.getNumPages())
    {
        std::string indices{};
        for (auto nodeIndex : nodeIndices)
        {
            indices += " " + std::to_string(nodeIndex);
        }
        throw std::invalid_argument(
            "Illegal indices for reading NTUPLES node: " + indices);
    }

    // map PAGE
    return mapNTuplesPage(nTuples.getPage(nodeIndices.at(0)), isPeakData);
}

sciformats::api::Node sciformats::jdx::api::JdxConverter::mapNTuplesPage(
    const Page& page, bool isPeakData)
{
    auto name = mapNTuplesPageName(page);

    std::vector<sciformats::api::KeyValueParam> parameters{};
    for (auto&& ldr : page.getPageLdrs())
    {
        parameters.push_back({ldr.getLabel(), ldr.getValue()});
    }

    std::vector<sciformats::api::Point2D> data{};
    sciformats::api::Table peakTable{};
    if (page.getDataTable())
    {
        auto dataTable = page.getDataTable().value();
        if (dataTable.getPlotDescriptor().has_value())
        {
            parameters.push_back(
                {"Plot Descriptor", dataTable.getPlotDescriptor().value()});
        }
        data = mapXyData(dataTable.getData());

        if (isPeakData)
        {
            peakTable = mapDataAsPeakTable(dataTable.getData());
        }
    }

    std::vector<std::string> childNodeNames{};

    auto metadata = mapMetadata(page, isPeakData);

    return {name, parameters, data, metadata, peakTable, childNodeNames};
}

std::string sciformats::jdx::api::JdxConverter::mapNTuplesPageName(
    const Page& page)
{
    auto name = page.getPageVariables();
    if (page.getDataTable())
    {
        const auto& dataTable = page.getDataTable().value();
        name += " - " + dataTable.getAttributes().yAttributes.varName;
    }
    return name;
}

std::vector<sciformats::api::Point2D>
sciformats::jdx::api::JdxConverter::mapData(const Block& block)
{
    std::optional<std::vector<std::pair<double, double>>> rawData{};
    if (block.getXyData())
    {
        // TODO: fix const definitions that prevent rawData =
        // block.getXyData().value().getData();
        auto data = block.getXyData().value();
        rawData = data.getData();
    }
    if (block.getRaData())
    {
        auto data = block.getRaData().value();
        rawData = data.getData();
    }
    if (block.getXyPoints())
    {
        auto data = block.getXyPoints().value();
        rawData = data.getData();
    }

    return rawData ? mapXyData(rawData.value())
                   : std::vector<sciformats::api::Point2D>{};
}

std::vector<sciformats::api::Point2D>
sciformats::jdx::api::JdxConverter::mapXyData(
    const std::vector<std::pair<double, double>>& xyData)
{
    std::vector<sciformats::api::Point2D> output{};
    output.reserve(xyData.size());
    for (const auto& pair : xyData)
    {
        output.push_back({pair.first, pair.second});
    }

    return output;
}

std::map<std::string, std::string>
sciformats::jdx::api::JdxConverter::mapMetadata(const Block& block)
{
    std::map<std::string, std::string> metadata{};

    if (auto xUnits = block.getLdr("XUNITS"))
    {
        metadata.emplace("x.unit", xUnits.value().getValue());
    }
    else if (auto rUnits = block.getLdr("RUNITS"))
    {
        metadata.emplace("x.unit", rUnits.value().getValue());
    }

    if (metadata.count("x.unit") > 0)
    {
        auto xUnit = metadata.at("x.unit");
        util::toLower(xUnit);
        if (xUnit == "1/cm" || xUnit == "1 / cm" || xUnit == "cm-1"
            || xUnit == "cm^-1")
        {
            metadata.emplace("x.label", "Wavenumber");

            if (block.getLdr("DATATYPE"))
            {
                auto dataType = block.getLdr("DATATYPE").value().getValue();
                util::toLower(dataType);

                if (dataType.find("infrared") != std::string::npos
                    || dataType.find("raman") != std::string::npos)
                {
                    metadata.emplace("x.reverse", "true");
                }
            }
        }
        else if (xUnit == "nanometers" || xUnit == "micrometers"
                 || xUnit == "nm" || xUnit == "um")
        {
            metadata.emplace("x.label", "Wavelength");
        }
    }

    if (auto yUnits = block.getLdr("YUNITS"))
    {
        metadata.emplace("y.unit", yUnits.value().getValue());
    }
    else if (auto aUnits = block.getLdr("AUNITS"))
    {
        metadata.emplace("y.unit", aUnits.value().getValue());
    }

    if (isPeakData(block))
    {
        metadata.emplace("plot.style", "sticks");
    }

    return metadata;
}

std::map<std::string, std::string>
sciformats::jdx::api::JdxConverter::mapMetadata(
    const Page& page, bool isPeakData)
{
    std::map<std::string, std::string> metadata{};

    if (page.getDataTable().has_value())
    {
        const auto& pageAttributes
            = page.getDataTable().value().getAttributes();
        if (pageAttributes.xAttributes.units.has_value())
        {
            metadata.emplace(
                "x.unit", pageAttributes.xAttributes.units.value());
        }
        if (pageAttributes.yAttributes.units.has_value())
        {
            metadata.emplace(
                "y.unit", pageAttributes.yAttributes.units.value());
        }
        if (isPeakData)
        {
            metadata.emplace("plot.style", "sticks");
        }
        else
        {
            // don't use VAR_NAMEs for labels for MS
            metadata.emplace("x.label", pageAttributes.xAttributes.symbol);
            metadata.emplace("y.label", pageAttributes.yAttributes.symbol);
        }
    }

    return metadata;
}

sciformats::api::Table sciformats::jdx::api::JdxConverter::mapPeakTable(
    const sciformats::jdx::PeakTable& peakTable)
{
    auto peakTableData = peakTable.getData();
    const auto has_w = std::any_of(peakTableData.cbegin(), peakTableData.cend(),
        [](const Peak& peak) { return peak.w.has_value(); });
    const auto has_m = std::any_of(peakTableData.cbegin(), peakTableData.cend(),
        [](const Peak& peak) { return peak.m.has_value(); });

    auto resultPeakTable = sciformats::api::Table{};
    resultPeakTable.columnNames.emplace_back("x", "Peak Position");
    resultPeakTable.columnNames.emplace_back("y", "Intensity");
    if (has_w)
    {
        resultPeakTable.columnNames.emplace_back("w", "Width");
    }
    if (has_m)
    {
        resultPeakTable.columnNames.emplace_back("m", "Multiplicity");
    }

    for (const auto& peak : peakTable.getData())
    {
        auto resultPeak = std::map<std::string, std::string>{};
        resultPeak.emplace("x", std::to_string(peak.x));
        resultPeak.emplace("y", std::to_string(peak.y));
        if (peak.w.has_value())
        {
            resultPeak.emplace("w", std::to_string(peak.w.value()));
        }
        if (peak.m.has_value())
        {
            resultPeak.emplace("m", peak.m.value());
        }
        resultPeakTable.rows.push_back(resultPeak);
    }
    return resultPeakTable;
}

std::vector<sciformats::api::Point2D>
sciformats::jdx::api::JdxConverter::mapPeakTableAsData(
    const sciformats::jdx::PeakTable& peakTable)
{
    std::vector<sciformats::api::Point2D> data{};

    for (const auto& peak : peakTable.getData())
    {
        data.push_back({peak.x, peak.y});
    }

    return data;
}

sciformats::api::Table sciformats::jdx::api::JdxConverter::mapDataAsPeakTable(
    const std::vector<std::pair<double, double>>& xyData)
{
    auto resultPeakTable = sciformats::api::Table{};

    resultPeakTable.columnNames.emplace_back("x", "Peak Position");
    resultPeakTable.columnNames.emplace_back("y", "Intensity");

    for (const auto& xyPair : xyData)
    {
        auto resultPeak = std::map<std::string, std::string>{};
        resultPeak.emplace("x", std::to_string(xyPair.first));
        resultPeak.emplace("y", std::to_string(xyPair.second));
        resultPeakTable.rows.push_back(resultPeak);
    }

    return resultPeakTable;
}

sciformats::api::Table sciformats::jdx::api::JdxConverter::mapPeakAssignments(
    const sciformats::jdx::PeakAssignments& peakAssignments)
{
    auto peakAssignmentsData = peakAssignments.getData();
    const auto has_y = std::any_of(peakAssignmentsData.cbegin(),
        peakAssignmentsData.cend(), [](const PeakAssignment& peakAssignment) {
            return peakAssignment.y.has_value();
        });
    const auto has_m = std::any_of(peakAssignmentsData.cbegin(),
        peakAssignmentsData.cend(), [](const PeakAssignment& peakAssignment) {
            return peakAssignment.m.has_value();
        });
    const auto has_w = std::any_of(peakAssignmentsData.cbegin(),
        peakAssignmentsData.cend(), [](const PeakAssignment& peakAssignment) {
            return peakAssignment.w.has_value();
        });

    auto resultPeakTable = sciformats::api::Table{};
    resultPeakTable.columnNames.emplace_back("x", "Peak Position");
    if (has_y)
    {
        resultPeakTable.columnNames.emplace_back("y", "Intensity");
    }
    if (has_m)
    {
        resultPeakTable.columnNames.emplace_back("m", "Multiplicity");
    }
    if (has_w)
    {
        resultPeakTable.columnNames.emplace_back("w", "Width");
    }
    resultPeakTable.columnNames.emplace_back("a", "Assignment");

    for (const auto& peakAssignment : peakAssignments.getData())
    {
        auto resultPeak = std::map<std::string, std::string>{};
        resultPeak.emplace("x", std::to_string(peakAssignment.x));
        if (peakAssignment.y.has_value())
        {
            resultPeak.emplace("y", std::to_string(peakAssignment.y.value()));
        }
        if (peakAssignment.w.has_value())
        {
            resultPeak.emplace("w", std::to_string(peakAssignment.w.value()));
        }
        if (peakAssignment.m.has_value())
        {
            resultPeak.emplace("m", peakAssignment.m.value());
        }
        resultPeak.emplace("a", peakAssignment.a);
        resultPeakTable.rows.push_back(resultPeak);
    }
    return resultPeakTable;
}

sciformats::api::Node sciformats::jdx::api::JdxConverter::mapAuditTrail(
    const sciformats::jdx::AuditTrail& auditTrail)
{
    auto auditEntries = auditTrail.getData();

    const auto has_process = std::any_of(auditEntries.cbegin(),
        auditEntries.cend(),
        [](const AuditTrailEntry& entry) { return entry.process.has_value(); });
    const auto has_version = std::any_of(auditEntries.cbegin(),
        auditEntries.cend(),
        [](const AuditTrailEntry& entry) { return entry.version.has_value(); });

    sciformats::api::Table table{};
    // columns
    table.columnNames.emplace_back("number", "NUMBER");
    table.columnNames.emplace_back("when", "WHEN");
    table.columnNames.emplace_back("who", "WHO");
    table.columnNames.emplace_back("where", "WHERE");
    if (has_process)
    {
        table.columnNames.emplace_back("process", "PROCESS");
    }
    if (has_version)
    {
        table.columnNames.emplace_back("version", "VERSION");
    }
    table.columnNames.emplace_back("what", "WHAT");

    // rows
    for (const auto& entry : auditEntries)
    {
        std::map<std::string, std::string> row{
            {"number", std::to_string(entry.number)},
            {"when", entry.when},
            {"who", entry.who},
            {"where", entry.where},
            {"what", entry.what},
        };
        if (has_process && entry.process.has_value())
        {
            row.emplace("process", entry.process.value());
        }
        if (has_version && entry.version.has_value())
        {
            row.emplace("version", entry.version.value());
        }

        table.rows.push_back(row);
    }

    const auto& auditTrailLabel = auditTrail.getLabel();

    return {
        auditTrailLabel,
        std::vector<sciformats::api::KeyValueParam>{},
        std::vector<sciformats::api::Point2D>{},
        std::map<std::string, std::string>{},
        table,
        std::vector<std::string>{},
    };
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(JdxConverter)
{
    using namespace sciformats::api;
    using namespace sciformats::jdx::api;
    using namespace emscripten;
    class_<JdxConverter, base<Converter>>("JdxConverter")
        .smart_ptr_constructor(
            "JdxConverter", &std::make_shared<JdxConverter, const std::string&>)
        .function("read", &JdxConverter::read);
}
#endif
