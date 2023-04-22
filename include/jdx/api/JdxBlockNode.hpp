#ifndef LIBJDX_API_JDXBLOCKNODE_HPP
#define LIBJDX_API_JDXBLOCKNODE_HPP

#include "api/KeyValueParam.hpp"
#include "api/Node.hpp"
#include "jdx/Block.hpp"

namespace sciformats::jdx::api
{
class JdxBlockNode : public sciformats::api::Node
{
public:
    explicit JdxBlockNode(std::unique_ptr<std::istream> stream);
    explicit JdxBlockNode(const sciformats::jdx::Block& block);
    [[nodiscard]] std::string getName() const override;
    std::vector<sciformats::api::KeyValueParam> getParams() override;
    std::vector<sciformats::api::Point2D> getData() override;
    std::vector<std::shared_ptr<sciformats::api::Node>>
    getChildNodes() override;

private:
    std::optional<sciformats::jdx::Block> m_block;
    const sciformats::jdx::Block& m_blockRef;
};
} // sciformats::jdx::api

#endif // LIBJDX_API_JDXBLOCKNODE_HPP
