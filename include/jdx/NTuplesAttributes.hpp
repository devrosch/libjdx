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
#ifndef JDX_NTUPLESATTRIBUTES_HPP
#define JDX_NTUPLESATTRIBUTES_HPP

#include "jdx/StringLdr.hpp"

#include <optional>
#include <vector>

namespace libjdx::jdx
{
/**
 * @brief A collection of attributes describing NTUPLES data.
 */
struct NTuplesAttributes
{
public:
    /**
     * @brief VAR_NAME.
     */
    std::string varName;
    /**
     * @brief SYMBOL.
     */
    std::string symbol;
    /**
     * @brief VAR_TYPE.
     */
    std::optional<std::string> varType;
    /**
     * @brief VAR_FORM.
     */
    std::optional<std::string> varForm;
    /**
     * @brief VAR_DIM.
     */
    std::optional<uint64_t> varDim; // optional, may be blank for mass spectra
    /**
     * @brief UNITS.
     */
    std::optional<std::string> units;
    /**
     * @brief FIRST.
     */
    std::optional<double> first;
    /**
     * @brief LAST.
     */
    std::optional<double> last;
    /**
     * @brief MIN.
     */
    std::optional<double> min;
    /**
     * @brief MAX.
     */
    std::optional<double> max;
    /**
     * @brief FACTOR.
     */
    std::optional<double> factor;
    /**
     * @brief Additional application specific LDRs.
     */
    std::vector<StringLdr> applicationAttributes;
};
} // namespace libjdx::jdx

#endif // JDX_NTUPLESATTRIBUTES_HPP
