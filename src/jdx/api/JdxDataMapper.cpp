#include "jdx/api/JdxDataMapper.hpp"
#include "api/Node.hpp"
#include "api/Point2D.hpp"
#include "jdx/JdxParser.hpp"
#include "jdx/api/JdxBlockNode.hpp"
#include "util/StringUtils.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

sciformats::jdx::api::JdxDataMapper::JdxDataMapper(const std::string& path)
    : m_rootBlock{std::make_unique<sciformats::jdx::Block>(
        sciformats::jdx::JdxParser::parse(path))}
{
}

sciformats::api::Node2 sciformats::jdx::api::JdxDataMapper::read(
    const std::string& path)
{
    std::cout << "C++: JdxDataMapper.read(): " << path << '\n';
    std::vector<size_t> nodeIndices = convertPathToNodeIndices(path);
    auto node = retrieveNode(nodeIndices);
    return node;
}

std::vector<size_t>
sciformats::jdx::api::JdxDataMapper::convertPathToNodeIndices(
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

sciformats::api::Node2 sciformats::jdx::api::JdxDataMapper::retrieveNode(
    const std::vector<size_t>& nodeIndices)
{
    const Block* block = &(*m_rootBlock);
    size_t iterationIndex = 0;
    for (const auto nodeIndex : nodeIndices)
    {
        if (nodeIndex == 0 && block->getNTuples().has_value())
        {
            // consider NTUPLES LDR as first child node
            auto startIt = nodeIndices.cbegin() + nodeIndex;
            auto nTuplesIndices
                = std::vector<size_t>{++startIt, nodeIndices.cend()};
            return mapNTuples(block->getNTuples().value(), nTuplesIndices);
        }
        const auto& childBlock = block->getBlocks().at(nodeIndex);
        block = &childBlock;
        ++iterationIndex;
    }
    return mapBlock(*block);
}

sciformats::api::Node2 sciformats::jdx::api::JdxDataMapper::mapBlock(
    const Block& block)
{
    auto name = block.getLdr("TITLE").value().getValue();

    std::vector<sciformats::api::KeyValueParam> parameters{};
    for (auto const& ldr : block.getLdrs())
    {
        parameters.push_back({ldr.getLabel(), ldr.getValue()});
    }

    auto data = mapData(block);

    std::vector<std::string> childNodeNames{};
    if (block.getNTuples().has_value())
    {
        // consider NTUPLES LDR as first child node
        auto nTuplesName = block.getNTuples().value().getDataForm();
        childNodeNames.push_back(nTuplesName);
    }
    for (auto const& childBlock : block.getBlocks())
    {
        auto childNodeName = childBlock.getLdr("TITLE").value().getValue();
        childNodeNames.push_back(childNodeName);
    }

    return {name, parameters, data, childNodeNames};
}

sciformats::api::Node2 sciformats::jdx::api::JdxDataMapper::mapNTuples(
    const NTuples& nTuples, const std::vector<size_t>& nodeIndices)
{
    if (nodeIndices.empty())
    {
        // map NTUPLES record
        auto name = nTuples.getDataForm();
        std::vector<sciformats::api::KeyValueParam> parameters{};
        size_t parameterColumnIndex = 0;
        for (auto&& attr : nTuples.getAttributes())
        {
            auto params = mapNTuplesAttributes(attr);
            for (auto&& param : params)
            {
                parameters.push_back({std::string{"column "}
                                          + std::to_string(parameterColumnIndex)
                                          + " - " + param.key,
                    param.value});
            }
            ++parameterColumnIndex;
        }
        std::vector<sciformats::api::Point2D> data{};
        std::vector<std::string> childNodeNames{};
        for (size_t pageIndex = 0; pageIndex < nTuples.getNumPages();
             ++pageIndex)
        {
            const auto& page = nTuples.getPage(pageIndex);
            childNodeNames.push_back(page.getPageVariables());
        }
        return {name, parameters, data, childNodeNames};
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
    return mapNTuplesPage(nTuples.getPage(nodeIndices.at(0)));
}

sciformats::api::Node2 sciformats::jdx::api::JdxDataMapper::mapNTuplesPage(
    const Page& page)
{
    const auto& name = page.getPageVariables();
    std::vector<sciformats::api::KeyValueParam> parameters{};
    for (auto&& ldr : page.getPageLdrs())
    {
        parameters.push_back({ldr.getLabel(), ldr.getValue()});
    }
    std::vector<sciformats::api::Point2D> data{};
    if (page.getDataTable())
    {
        auto dataTable = page.getDataTable().value();
        if (dataTable.getPlotDescriptor().has_value())
        {
            parameters.push_back(
                {"Plot Descriptor", dataTable.getPlotDescriptor().value()});
        }
        auto attributes = dataTable.getAttributes();
        auto xParams = mapNTuplesAttributes(attributes.xAttributes);
        auto yParams = mapNTuplesAttributes(attributes.yAttributes);
        parameters.insert(parameters.end(), xParams.cbegin(), xParams.cend());
        parameters.insert(parameters.end(), yParams.cbegin(), yParams.cend());

        data = mapXyData(dataTable.getData());
    }
    std::vector<std::string> childNodeNames{};

    return {name, parameters, data, childNodeNames};
}

std::vector<sciformats::api::KeyValueParam>
sciformats::jdx::api::JdxDataMapper::mapNTuplesAttributes(
    const NTuplesAttributes& nTuplesAttributes)
{
    std::vector<sciformats::api::KeyValueParam> parameters{};
    // map standard attributes
    parameters.push_back({"VAR_NAME", nTuplesAttributes.varName});
    parameters.push_back({"SYMBOL", nTuplesAttributes.symbol});
    if (nTuplesAttributes.varType)
    {
        parameters.push_back({"VAR_TYPE", nTuplesAttributes.varType.value()});
    }
    if (nTuplesAttributes.varForm)
    {
        parameters.push_back({"VAR_FORM", nTuplesAttributes.varForm.value()});
    }
    if (nTuplesAttributes.varDim)
    {
        parameters.push_back(
            {"VAR_DIM", std::to_string(nTuplesAttributes.varDim.value())});
    }
    if (nTuplesAttributes.units)
    {
        parameters.push_back({"UNITS", nTuplesAttributes.units.value()});
    }
    if (nTuplesAttributes.first)
    {
        parameters.push_back(
            {"FIRST", std::to_string(nTuplesAttributes.first.value())});
    }
    if (nTuplesAttributes.last)
    {
        parameters.push_back(
            {"LAST", std::to_string(nTuplesAttributes.last.value())});
    }
    if (nTuplesAttributes.min)
    {
        parameters.push_back(
            {"MIN", std::to_string(nTuplesAttributes.min.value())});
    }
    if (nTuplesAttributes.max)
    {
        parameters.push_back(
            {"MAX", std::to_string(nTuplesAttributes.max.value())});
    }
    if (nTuplesAttributes.factor)
    {
        parameters.push_back(
            {"FACTOR", std::to_string(nTuplesAttributes.factor.value())});
    }

    // map application attributes
    for (auto&& ldr : nTuplesAttributes.applicationAttributes)
    {
        parameters.push_back({ldr.getLabel(), ldr.getValue()});
    }

    return parameters;
}

std::vector<sciformats::api::Point2D>
sciformats::jdx::api::JdxDataMapper::mapData(const Block& block)
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
    // TODO: add PEAK TABLE, PEAK ASSIGNMENTS

    return rawData ? mapXyData(rawData.value())
                   : std::vector<sciformats::api::Point2D>{};
}

std::vector<sciformats::api::Point2D>
sciformats::jdx::api::JdxDataMapper::mapXyData(
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

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(JdxDataMapper)
{
    using namespace sciformats::api;
    using namespace sciformats::jdx::api;
    using namespace emscripten;
    class_<JdxDataMapper, base<DataMapper>>("JdxDataMapper")
        .smart_ptr_constructor("JdxDataMapper",
            &std::make_shared<JdxDataMapper, const std::string&>)
        .function("read", &JdxDataMapper::read);
}
#endif
