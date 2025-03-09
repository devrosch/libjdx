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
#ifndef JDX_STRINGLDR_HPP
#define JDX_STRINGLDR_HPP

#include "jdx/Ldr.hpp"

#include <string>

namespace sciformats::jdx
{
/**
 * @brief A JCAMP-DX labeled data record (LDR).
 */
class StringLdr : public Ldr
{
public:
    /**
     * @brief Constructs a Ldr from label and string value.
     * @param label The label of the LDR, e.g. "TITLE" for "##TITLE= abc".
     * @param value The value of the LDR, e.g. "abc" for "##TITLE= abc".
     */
    StringLdr(std::string label, std::string value);

    /**
     * @brief The value (without initial blank character if any) of the
     * LDR, e.g. "abc" for "##TITLE= abc".
     * @return The value of the LDR. If the value spans multiple lines, the
     * return value contains all lines, separated by \"\\n\".
     */
    [[nodiscard]] const std::string& getValue() const;

private:
    const std::string m_value;
};
} // namespace sciformats::jdx

#endif // JDX_STRINGLDR_HPP
