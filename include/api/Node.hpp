#ifndef API_NODE_HPP
#define API_NODE_HPP

#include "api/KeyValueParam.hpp"
#include "api/Point2D.hpp"

#include <string>
#include <vector>

namespace sciformats::api
{
struct Node
{
    std::string name;
    std::vector<KeyValueParam> parameters;
    std::vector<Point2D> data;
    std::vector<std::string> childNodeNames;
};
} // sciformats::api

#endif
