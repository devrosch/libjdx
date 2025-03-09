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
#ifndef JDX_LDR_HPP
#define JDX_LDR_HPP

#include <string>

namespace libjdx::jdx
{
/**
 * @brief A JCAMP-DX labeled data record (LDR).
 */
class Ldr
{
public:
    /**
     * @brief Constructs a Ldr from label and string value.
     * @param label The label of the LDR, e.g. "TITLE" for "##TITLE= abc".
     */
    explicit Ldr(std::string label);

    /**
     * @brief The label of the LDR, e.g. "TITLE" for "##TITLE= abc".
     * @return The label of the LDR.
     */
    [[nodiscard]] const std::string& getLabel() const;

    /**
     * @brief Whether LDR is user defined, i.e. the label starts
     * with "$", e.g. "##$USER_DEFINED_LABEL= abc".
     * @return "true" if label is user defined, otherwise "false".
     */
    [[nodiscard]] bool isUserDefined() const;

    /**
     * @brief Whether LDR is technique specific, i.e. the label starts
     * with ".", e.g. "##.OBSERVE_FREQUENCY= 50.0".
     * @return "true" if label is user defined, otherwise "false".
     */
    [[nodiscard]] bool isTechniqueSpecific() const;

private:
    const std::string m_label;
};
} // namespace libjdx::jdx

#endif // JDX_LDR_HPP
