#ifndef API_DATASCANNER_HPP
#define API_DATASCANNER_HPP

#include "api/Node.hpp"

#include <string>

namespace sciformats::api
{
/**
 * Scan raw data to determine if a parser is applicable.
 */
class DataScanner
{
public:
    /**
     * @brief Whether a data set is recognized.
     * @param path The path to the data set.
     * @return True if recognized, false otherwise.
     */
    virtual bool isRecognized(const std::string& path) = 0;

    // https://stackoverflow.com/questions/26039907/does-rule-of-three-five-apply-to-inheritance-and-virtual-destructors
    DataScanner() = default;
    DataScanner(const DataScanner& node) = default;
    DataScanner& operator=(const DataScanner& node) = default;
    DataScanner(DataScanner&&) = default;
    DataScanner& operator=(DataScanner&&) = default;
    virtual ~DataScanner() = default;
};
} // sciformats::api

#endif
