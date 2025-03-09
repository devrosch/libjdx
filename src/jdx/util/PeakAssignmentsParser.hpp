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
#ifndef JDX_PEAKASSIGNMENTSPARSER_HPP
#define JDX_PEAKASSIGNMENTSPARSER_HPP

#include "io/TextReader.hpp"
#include "jdx/PeakAssignment.hpp"
#include "util/MultilineTuplesParser.hpp"

#include <array>

namespace libjdx::jdx::util
{
/**
 * @brief A parser for PEAK ASSIGNMENTS.
 */
class PeakAssignmentsParser : protected MultilineTuplesParser
{
public:
    explicit PeakAssignmentsParser(
        io::TextReader& reader, std::string variableList);

    /**
     * @brief Next peak assignment.
     * @note Assumes that a peak assignment tuple always starts on a new line,
     * but may span multiple lines.
     * @return The next peak assignment, nullopt if there is none.
     * @throws ParseException If next peak assignment is malformed.
     */
    std::optional<PeakAssignment> next();

private:
    static constexpr const char* s_ldrName = "peak assignments";

    static constexpr std::array<const char*, 4> s_varLists = {
        "(XYA)",
        "(XYWA)",
        "(XYMA)",
        "(XYMWA)",
    };

    /**
     * matches 2 - 5 peak assignments segments  as groups 1-5, corresponding to
     * one of (X[, Y][, W], A), (X[, Y][, M], A), (X[, Y][, M][, W], A), with X
     * as matches[1] and A as matches[5]
     */
    const std::regex m_regex{R"(^\s*\(\s*)"
                             R"(([^,]*))"
                             R"((?:\s*,\s*([^,]*))?)"
                             R"((?:\s*,\s*([^,]*))?)"
                             R"((?:\s*,\s*([^,]*))?)"
                             R"(\s*,\s*<(.*)>\s*\))"
                             R"(\s*$)"};

    [[nodiscard]] libjdx::jdx::PeakAssignment createPeakAssignment(
        const std::string& tuple) const;
};
}

#endif /* JDX_PEAKASSIGNMENTSPARSER_HPP */
