#ifndef LIBJDX_API_JDXDATA2DNODE_HPP
#define LIBJDX_API_JDXDATA2DNODE_HPP

#include "api/Node.hpp"
#include "api/Point2D.hpp"

#include <variant>

namespace sciformats::jdx::api
{
class JdxData2DNode : public sciformats::api::Node
{
public:
    explicit JdxData2DNode(
        std::string name, std::vector<std::pair<double, double>> data);
    [[nodiscard]] std::string getName() const override;
    std::vector<sciformats::api::KeyValueParam> getParams() override;
    std::optional<std::vector<sciformats::api::Point2D>> getData() override;
    std::vector<std::shared_ptr<sciformats::api::Node>>
    getChildNodes() override;

protected:
    static std::vector<sciformats::api::Point2D> mapPairsToPoints(
        const std::vector<std::pair<double, double>>& data);

private:
    const std::string m_name;
    const std::vector<std::pair<double, double>> m_data;
};
} // sciformats::jdx::api

#endif // LIBJDX_API_JDXDATA2DNODE_HPP
