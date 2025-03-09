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
#ifndef API_CONVERTER_HPP
#define API_CONVERTER_HPP

#include "api/Node.hpp"

#include <memory>
#include <string>

namespace libjdx::api
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
} // libjdx::api

#endif
