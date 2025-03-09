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
#ifndef JDX_PEAKASSIGNMENT_HPP
#define JDX_PEAKASSIGNMENT_HPP

#include <optional>
#include <string>

namespace sciformats::jdx
{
/**
 * @brief A JCAMP-DX peak assignment, i.e., one item in PEAK ASSIGNMENTS.
 */
struct PeakAssignment
{
    /**
     * @brief Peak position.
     */
    double x{};

    /**
     * @brief Intensity.
     */
    std::optional<double> y; // standard is ambiguous whether this is optional

    /**
     * @brief Multiplicity.
     * @remark S, D, Т, Q for singlets, douЬlets, triplets, or quadruplets, М
     * for multiple, and U for unassigned. Used only for NMR.
     */
    std::optional<std::string> m;

    /**
     * @brief Width.
     */
    std::optional<double> w;

    /**
     * @brief The peak assignment string.
     */
    std::string a;
};
} // namespace sciformats::jdx

#endif // JDX_PEAKASSIGNMENT_HPP
