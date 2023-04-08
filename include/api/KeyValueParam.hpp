#ifndef LIBAPI_KEYVALUEPARAM_HPP
#define LIBAPI_KEYVALUEPARAM_HPP

#include <string>

namespace sciformats::api
{
struct KeyValueParam
{
    std::string key;
    std::string value;
};
} // sciformats::api

#endif // LIBAPI_KEYVALUEPARAM_HPP
