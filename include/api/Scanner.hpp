#ifndef API_SCANNER_HPP
#define API_SCANNER_HPP

#include "api/Converter.hpp"

#include <string>

namespace sciformats::api
{
/**
 * Scan raw data to determine if a converter is applicable and provide
 * converter.
 */
class Scanner
{
public:
    /**
     * @brief Whether a data set is recognized. Shallow check.
     * @param path The path to the data set.
     * @return True if recognized, false otherwise.
     */
    virtual bool isRecognized(const std::string& path) = 0;

    /**
     * @brief Get the converter for the data set.
     * @param path The path to the data set.
     * @return The converter for the data set.
     */
    virtual std::unique_ptr<Converter> getConverter(const std::string& path)
        = 0;

    // https://stackoverflow.com/questions/26039907/does-rule-of-three-five-apply-to-inheritance-and-virtual-destructors
    Scanner() = default;
    Scanner(const Scanner& scanner) = default;
    Scanner& operator=(const Scanner& scanner) = default;
    Scanner(Scanner&&) = default;
    Scanner& operator=(Scanner&&) = default;
    virtual ~Scanner() = default;
};
} // sciformats::api

#endif
