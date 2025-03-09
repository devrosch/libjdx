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
#include "jdx/Ldr.hpp"

libjdx::jdx::Ldr::Ldr(std::string label)
    : m_label{std::move(label)}
{
}

const std::string& libjdx::jdx::Ldr::getLabel() const
{
    return m_label;
}

bool libjdx::jdx::Ldr::isUserDefined() const
{
    return !m_label.empty() && m_label.at(0) == '$';
}

bool libjdx::jdx::Ldr::isTechniqueSpecific() const
{
    return !m_label.empty() && m_label.at(0) == '.';
}
