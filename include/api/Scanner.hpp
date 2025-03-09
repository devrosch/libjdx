/* Copyright (C) 2025 Robert Schiwon
 *
 * This file is part of libjdx.
 *
 * libjdx is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * libjdx is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libjdx. If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef API_SCANNER_HPP
#define API_SCANNER_HPP

#include "api/Converter.hpp"

#include <string>

namespace libjdx::api
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
} // libjdx::api

#endif
