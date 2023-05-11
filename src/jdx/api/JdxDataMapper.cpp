#include "jdx/api/JdxDataMapper.hpp"
#include "api/Node.hpp"
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
  : m_rootBlock{std::make_unique<sciformats::jdx::Block>(sciformats::jdx::JdxParser::parse(path))}
{
}

sciformats::api::Node2
sciformats::jdx::api::JdxDataMapper::read(const std::string& path)
{
    std::cout << "C++: JdxDataMapper.read(): " << path << '\n';
    std::vector<size_t> nodeIndices = convertPathToNodeIndices(path);
    auto node = retrieveNode(nodeIndices);
    return node;
}

std::vector<size_t> sciformats::jdx::api::JdxDataMapper::convertPathToNodeIndices(const std::string& path)
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

sciformats::api::Node2 sciformats::jdx::api::JdxDataMapper::retrieveNode(const std::vector<size_t>& nodeIndices)
{
    // TODO: try and find a way to do this without raw pointers
    const Block* block = &(*m_rootBlock);
    for (const auto nodeIndex : nodeIndices)
    {
        const auto& childBlock = block->getBlocks().at(nodeIndex);
        block = &childBlock;
    }
    return mapBlock(*block);
}

sciformats::api::Node2 sciformats::jdx::api::JdxDataMapper::mapBlock(const Block& block)
{
    auto name = block.getLdr("TITLE").value().getValue();

    std::vector<sciformats::api::KeyValueParam> parameters{};
    for (auto const& ldr : block.getLdrs())
    {
        parameters.push_back({ldr.getLabel(), ldr.getValue()});
    }
    
    auto data = mapData(block);

    std::vector<std::string> childNodeNames{};
    for (auto const& childBlock : block.getBlocks())
    {
        auto childNodeName = childBlock.getLdr("TITLE").value().getValue();
        childNodeNames.push_back(childNodeName);
    }

    return { name, parameters, data, childNodeNames };
}

std::vector<sciformats::api::Point2D> sciformats::jdx::api::JdxDataMapper::mapData(const Block& block)
{
    std::optional<std::vector<std::pair<double, double>>> rawData{};
    if (block.getXyData())
    {
        // TODO: fix const definitions that prevent rawData = block.getXyData().value().getData();
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
    // TODO: add PEAK TABLE, PEAK ASSIGNMENTS, NTUPLES

    std::vector<sciformats::api::Point2D> output{};
    if (rawData)
    {
        output.reserve(rawData.value().size());
        for (const auto& pair : rawData.value())
        {
            output.push_back({pair.first, pair.second});
        }
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
        .smart_ptr_constructor(
            "JdxDataMapper", &std::make_shared<JdxDataMapper, const std::string&>)
        .function("read", &JdxDataMapper::read);
}
#endif
