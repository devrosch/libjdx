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
#ifndef JDX_PEAKASSIGNMENTS_HPP
#define JDX_PEAKASSIGNMENTS_HPP

#include "io/TextReader.hpp"
#include "jdx/DataLdr.hpp"
#include "jdx/PeakAssignment.hpp"
#include "jdx/TabularData.hpp"

#include <array>
#include <optional>
#include <string>
#include <vector>

namespace libjdx::jdx
{
/**
 * @brief A JCAMP-DX PEAK ASSIGNMENTS record.
 */
class PeakAssignments : public TabularData
{
public:
    /**
     * @brief PeakAssignments Constructs PeakAssignments.
     * @param label The label of the first line of the record, i.e.
     * "PEAKASSIGNMENTS".
     * @param variableList The value of the first line of the record
     * representing the structure of the data, e.g., "(XYA)".
     * @param reader Text reader with JCAMP-DX data. The reader position is
     * assumed to be at the start of the second line (the line following the
     * line containing the label) of the record. The reader is expected to exist
     * for the lifetime of this object.
     * @param nextLine The first line of the LDR, i.e., the one containing the
     * label. Will contain the line following the record or nullopt if the end
     * of the reader has been reached.
     */
    PeakAssignments(const std::string& label, std::string variableList,
        io::TextReader& reader, std::optional<std::string>& nextLine);

    /**
     * @brief Provides the parsed peak assignments.
     * @return The list of peak assignments.
     */
    [[nodiscard]] std::vector<PeakAssignment> getData() const;

private:
    static constexpr const char* s_peakAssignentsLabel = "PEAKASSIGNMENTS";
    static constexpr std::array<const char*, 4> s_peakAssignentsVariableLists
        = {"(XYA)", "(XYWA)", "(XYMA)", "(XYMWA)"};
};
} // namespace libjdx::jdx

#endif // JDX_PEAKASSIGNMENTS_HPP
