#include "jdx/api/JdxData2DNode.hpp"
#include "api/Point2D.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

sciformats::jdx::api::JdxData2DNode::JdxData2DNode(
    std::string name, std::vector<std::pair<double, double>> data)
    : m_name{std::move(name)}
    , m_data{std::move(data)}
{
}

std::string sciformats::jdx::api::JdxData2DNode::getName() const
{
    return m_name;
}

std::vector<sciformats::api::KeyValueParam>
sciformats::jdx::api::JdxData2DNode::getParams()
{
    return std::vector<sciformats::api::KeyValueParam>{};
}

std::optional<std::vector<sciformats::api::Point2D>>
sciformats::jdx::api::JdxData2DNode::getData()
{
    // TODO: avoid copy on each call
    return mapPairsToPoints(m_data);
}

std::vector<std::shared_ptr<sciformats::api::Node>>
sciformats::jdx::api::JdxData2DNode::getChildNodes()
{
    return {};
}

std::vector<sciformats::api::Point2D>
sciformats::jdx::api::JdxData2DNode::mapPairsToPoints(
    const std::vector<std::pair<double, double>>& data)
{
    std::vector<sciformats::api::Point2D> output{};
    output.reserve(data.size());
    for (const auto& pair : data)
    {
        output.push_back({pair.first, pair.second});
    }
    return output;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(JdxData2DNode)
{
    using namespace sciformats::api;
    using namespace sciformats::jdx::api;
    using namespace emscripten;
    // see: https://github.com/emscripten-core/emscripten/issues/627
    class_<JdxData2DNode, base<Node>>("JdxData2DNode")
        .property("name", &JdxData2DNode::getName)
        // embind fails mapping getParams() or getChildNodes() to a property
        .function("getParams", &JdxData2DNode::getParams)
        .function("getData", &JdxData2DNode::getData)
        .function("getChildNodes", &JdxData2DNode::getChildNodes);
}
#endif
