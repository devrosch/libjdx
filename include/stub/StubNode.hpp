#ifndef LIBSCIWRAP_STUBNODE_HPP
#define LIBSCIWRAP_STUBNODE_HPP

#include "api/KeyValueParam.hpp"
#include "api/Node.hpp"

namespace sciformats::stub
{
class StubNode : public sciformats::api::Node
{
public:
    [[nodiscard]] std::string getName() const override;
    std::vector<sciformats::api::KeyValueParam> getParams() override;
    std::optional<std::vector<sciformats::api::Point2D>> getData() override;
    std::vector<std::shared_ptr<sciformats::api::Node>>
    getChildNodes() override;

private:
};
} // sciformats::stub

#endif // LIBSCIWRAP_STUBNODE_HPP
