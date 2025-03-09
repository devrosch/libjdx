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
#ifndef JDX_RAPARAMETERS_HPP
#define JDX_RAPARAMETERS_HPP

#include <optional>
#include <string>

namespace sciformats::jdx
{
/**
 * @brief JCAMP-DX spectral parameters describing an RADATA record.
 *
 * It is poorly defined in the standard which elements are used for RADATA and
 * which are required and which are optional. The choice here is governed by
 * technical needs for parsing/displaying.
 */
struct RaParameters
{
    std::string rUnits; // not required for parsing but for displaying
    std::string aUnits; // not required for parsing but for displaying
    double firstR;
    double lastR;
    // no MAXR, MINR according to standard
    std::optional<double> maxA; // required, according to standard
    std::optional<double> minA; // required, according to standard
    double rFactor;
    double aFactor;
    uint64_t nPoints;
    std::optional<double> firstA;
    std::optional<double> resolution;
    std::optional<double> deltaR;
    std::optional<double> zdp;
    std::optional<std::string> alias; // standard says type is AFFN, but gives
                                      // "1/1" and "1/2" as examples
    // in addition, XUNITS, YUNITS, FIRSTX, LASTX, DELTAX are given in examples
    // in the standard with not quite clear meaning
};
} // namespace sciformats::jdx

#endif // JDX_RAPARAMETERS_HPP
