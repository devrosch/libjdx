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
#ifndef JDX_AUDITTRAILENTRY_HPP
#define JDX_AUDITTRAILENTRY_HPP

#include <optional>
#include <string>

namespace libjdx::jdx
{
/**
 * @brief A JCAMP-DX audit trail entry, i.e. one item in an AUDIT TRAIL.
 */
struct AuditTrailEntry
{
public:
    /**
     * @brief Change number.
     */
    std::int64_t number{};

    // TODO: Use std::chrono::time_point
    /**
     * @brief Timestamp.
     */
    std::string when{};

    /**
     * @brief Person who made or authorized the change.
     */
    std::string who{};

    /**
     * @brief Person’s location.
     */
    std::string where{};

    /**
     * @brief Process.
     */
    std::optional<std::string> process{};

    /**
     * @brief Software version.
     */
    std::optional<std::string> version{};

    /**
     * @brief Details of the change made.
     */
    std::string what{};
};
} // namespace libjdx::jdx

#endif // JDX_AUDITTRAILENTRY_HPP
