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
#ifndef JDX_LDRCONTAINER_HPP
#define JDX_LDRCONTAINER_HPP

#include "io/TextReader.hpp"

#include <optional>

namespace libjdx::jdx
{
/**
 * @brief Parent class of JCAMP-DX BLOCK and NTUPLES records.
 */
class LdrContainer
{
protected:
    static std::optional<const std::string> parseStringValue(
        std::string& value, io::TextReader& reader);
};
} // namespace libjdx::jdx

#endif // JDX_LDRCONTAINER_HPP
