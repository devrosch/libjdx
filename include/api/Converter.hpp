#ifndef API_CONVERTER_HPP
#define API_CONVERTER_HPP

#include "api/Node.hpp"

#include <memory>
#include <string>

namespace sciformats::api
{
/**
 * Map parsed data to a common structure.
 */
class Converter
{
public:
    /**
     * @brief Reads the Node from the data set.
     * @param path The path inside the data set identifying the Node.
     * @return The Node.
     */
    virtual Node read(const std::string& path) = 0;

    // https://stackoverflow.com/questions/26039907/does-rule-of-three-five-apply-to-inheritance-and-virtual-destructors
    Converter() = default;
    Converter(const Converter& node) = default;
    Converter& operator=(const Converter& node) = default;
    Converter(Converter&&) = default;
    Converter& operator=(Converter&&) = default;
    virtual ~Converter() = default;
};
} // sciformats::api

#endif
