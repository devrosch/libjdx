#ifndef LIBAPI_NODE_HPP
#define LIBAPI_NODE_HPP

#include "api/KeyValueParam.hpp"
#include "api/Point2D.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace sciformats::api
{
class Node
{
public:
    [[nodiscard]] virtual std::string getName() const = 0;
    virtual std::vector<KeyValueParam> getParams() = 0;
    virtual std::optional<std::vector<Point2D>> getData() = 0;
    virtual std::vector<std::shared_ptr<Node>> getChildNodes() = 0;

    // see:
    // https://stackoverflow.com/questions/26039907/does-rule-of-three-five-apply-to-inheritance-and-virtual-destructors
    // but also:
    // https://stackoverflow.com/questions/32557206/inheritance-from-a-pure-abstract-class-without-default-constructor
    Node() = default;
    Node(const Node& node) = default;
    Node& operator=(const Node& node) = default;
    Node(Node&&) = default;
    Node& operator=(Node&&) = default;
    virtual ~Node() = default;
};
} // sciformats::api

#endif // LIBAPI_NODE_HPP
