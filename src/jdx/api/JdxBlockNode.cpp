#include "jdx/api/JdxBlockNode.hpp"
#include "jdx/Block.hpp"
#include "jdx/api/JdxData2DNode.hpp"
#include "jdx/JdxParser.hpp"
#include "api/KeyValueParam.hpp"
#include "api/Node.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

sciformats::jdx::api::JdxBlockNode::JdxBlockNode(
    const sciformats::jdx::Block& block)
    : m_block{std::nullopt}
    , m_blockRef{block}
{
}

sciformats::jdx::api::JdxBlockNode::JdxBlockNode(
    std::unique_ptr<std::istream> stream)
    : m_block{sciformats::jdx::JdxParser::parse(std::move(stream))}
    , m_blockRef{m_block.value()}
{
}

std::string sciformats::jdx::api::JdxBlockNode::getName() const
{
    return m_blockRef.getLdr("TITLE").value().getValue();
}

std::vector<sciformats::api::KeyValueParam>
sciformats::jdx::api::JdxBlockNode::getParams()
{
    auto const& ldrs = m_blockRef.getLdrs();
    auto vec = std::vector<sciformats::api::KeyValueParam>();
    for (auto const& ldr : ldrs)
    {
        vec.push_back({ldr.getLabel(), ldr.getValue()});
    }
    return vec;
}

std::optional<std::vector<sciformats::api::Point2D>>
sciformats::jdx::api::JdxBlockNode::getData()
{
    // TODO: return data here for data blocks instead of as child node
    return std::nullopt;
}

std::vector<std::shared_ptr<sciformats::api::Node>>
sciformats::jdx::api::JdxBlockNode::getChildNodes()
{
    auto childNodes = std::vector<std::shared_ptr<Node>>();
    if (m_blockRef.getXyData())
    {
        auto data = m_blockRef.getXyData().value();
        auto dataPtr
            = std::make_shared<JdxData2DNode>("XYDATA", data.getData());
        childNodes.push_back(dataPtr);
    }
    if (m_blockRef.getRaData())
    {
        auto data = m_blockRef.getRaData().value();
        auto dataPtr
            = std::make_shared<JdxData2DNode>("RADATA", data.getData());
        childNodes.push_back(dataPtr);
    }
    if (m_blockRef.getXyPoints())
    {
        auto data = m_blockRef.getXyPoints().value();
        auto dataPtr
            = std::make_shared<JdxData2DNode>("XYPOINTS", data.getData());
        childNodes.push_back(dataPtr);
    }
    // TODO: add PEAK TABLE
    for (auto const& block : m_blockRef.getBlocks())
    {
        auto blockPtr = std::make_shared<JdxBlockNode>(block);
        childNodes.push_back(blockPtr);
    }
    // TODO: populate with other node types
    return childNodes;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(JdxBlockNode)
{
    using namespace sciformats::api;
    using namespace sciformats::jdx::api;
    using namespace emscripten;
    // see: https://github.com/emscripten-core/emscripten/issues/627
    class_<JdxBlockNode, base<Node>>("JdxBlockNode")
        //        .constructor<>()
        .property("name", &JdxBlockNode::getName)
        // embind fails mapping getParams() or getChildNodes() to a property
        .function("getParams", &JdxBlockNode::getParams)
        .function("getData", &JdxBlockNode::getData)
        .function("getChildNodes", &JdxBlockNode::getChildNodes);
}
#endif
