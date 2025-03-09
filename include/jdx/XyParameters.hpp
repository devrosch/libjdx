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
#ifndef JDX_XYPARAMETERS_HPP
#define JDX_XYPARAMETERS_HPP

#include <optional>
#include <string>

namespace sciformats::jdx
{
/**
 * @brief JCAMP-DX spectral parameters describing an XYDATA record.
 */
struct XyParameters
{
    // not required for parsing but for displaying
    std::string xUnits; //!< Abscissa units.
    // not required for parsing but for displaying
    std::string yUnits; //!< Ordinate units.
    double firstX; //!< The first X value.
    double lastX; //!< The last X value.
    std::optional<double> maxX;
    std::optional<double> minX;
    std::optional<double> maxY;
    std::optional<double> minY;
    /**
     * @brief The factor by which to multiply raw x values to arrive at the
     * actual value.
     */
    double xFactor;
    /**
     * @brief The factor by which to multiply raw y values to arrive at the
     * actual value.
     */
    double yFactor;
    /**
     * @brief The number of xy pairs in this record.
     */
    uint64_t nPoints;
    /**
     * @brief The first actual Y value (after scaling).
     */
    std::optional<double> firstY;
    /**
     * @brief The resolution of the data.
     */
    std::optional<double> resolution;
    /**
     * @brief The x distance between adjacent data points (if constant).
     */
    std::optional<double> deltaX;
};
} // namespace sciformats::jdx

#endif // JDX_XYPARAMETERS_HPP
