#ifndef API_DATAMAPPER_HPP
#define API_DATAMAPPER_HPP

#include "api/Node2.hpp"

#include <memory>
#include <string>

namespace sciformats::api
{
/**
 * Map parsed data to a common structure.
 */
class DataMapper
{
public:
    /**
     * @brief Reads the Node from the data set.
     * @param path The path inside the data set identifying the Node.
     * @return The Node.
     */
    virtual Node2 read(const std::string& path) = 0;

    // https://stackoverflow.com/questions/26039907/does-rule-of-three-five-apply-to-inheritance-and-virtual-destructors
    DataMapper() = default;
    DataMapper(const DataMapper& node) = default;
    DataMapper& operator=(const DataMapper& node) = default;
    DataMapper(DataMapper&&) = default;
    DataMapper& operator=(DataMapper&&) = default;
    virtual ~DataMapper() = default;
};
} // sciformats::api

#endif
