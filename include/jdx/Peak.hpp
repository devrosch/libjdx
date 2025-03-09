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
#ifndef JDX_PEAK_HPP
#define JDX_PEAK_HPP

#include <optional>

namespace sciformats::jdx
{
/**
 * @brief A JCAMP-DX peak, i.e. one item in a PEAK TABLE.
 */
struct Peak
{
public:
    /**
     * @brief x Peak position.
     */
    double x{};

    /**
     * @brief y Intensity.
     */
    double y{};

    /**
     * @brief Multiplicity.
     * @remark S, D, Т, Q for singlets, douЬlets, triplets, or quadruplets, М
     * for multiple, and U for unassigned. Used only for NMR.
     */
    std::optional<std::string> m;

    /**
     * @brief w Width.
     */
    std::optional<double> w;
};
} // namespace sciformats::jdx

#endif // JDX_PEAK_HPP
