#ifndef API_PEAKS_HPP
#define API_PEAKS_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace sciformats::api
{
/**
 * A peak table.
 */
struct PeakTable
{
    /**
     * A list of column keys and corresponding column names.
     */
    std::vector<std::pair<std::string, std::string>> columnNames;

    /**
     * A list of peaks.
     *
     * Each row represents a single peak and each key-value pair in the map
     * representing a single peak parameter (e.g., position, height). Only keys
     * from the coulmnNameMap may occur but not all keys from that list need to
     * occur as there may be missing values for a peak.
     */
    std::vector<std::map<std::string, std::string>> peaks;
};
} // sciformats::api

#endif
