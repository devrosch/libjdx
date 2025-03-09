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
#ifndef JDX_XYDATA_HPP
#define JDX_XYDATA_HPP

#include "io/TextReader.hpp"
#include "jdx/StringLdr.hpp"
#include "jdx/XyBase.hpp"
#include "jdx/XyParameters.hpp"

#include <array>
#include <vector>

namespace sciformats::jdx
{
/**
 * @brief A JCAMP-DX XYDATA record.
 */
class XyData : public XyBase
{
public:
    /**
     * @brief Constructs XyData from first line and reader.
     * @param label The label of the first line of the record, i.e. "XYDATA".
     * @param variableList The value of the first line of the record
     * representing the structure of the data, e.g., "(X++(Y..Y))".
     * @param parameters Parameters from the enclosing block specific to XYDATA.
     * @param reader Text reader with JCAMP-DX data. The reader position is
     * assumed to be at the start of the second line (the line following the
     * line containing the label) of the record. The reader is expected to exist
     * for the lifetime of this object.
     * @param nextLine The first line of the LDR, i.e., the one containing the
     * label. Will contain the line following the record or nullopt if the end
     * of the reader has been reached.
     */
    XyData(const std::string& label, const std::string& variableList,
        const std::vector<StringLdr>& ldrs, io::TextReader& reader,
        std::optional<std::string>& nextLine);

    /**
     * @brief Provides the parsed xy data.
     * @return Pairs of xy data. Invalid values ("?") will be represented by
     * std::numeric_limits<double>::quiet_NaN.
     */
    std::vector<std::pair<double, double>> getData();

private:
    static constexpr const char* s_xyDataLabel = "XYDATA";
    static constexpr const char* s_ooQuirkVarList = "(XY..XY)";
    // if more quirks need to be added, getData() must be changed
    static constexpr std::array<const char*, 4> s_xyDataVariableLists
        = {"(X++(Y..Y))", "(X++(R..R))", "(X++(I..I))", s_ooQuirkVarList};
};
} // namespace sciformats::jdx

#endif // JDX_XYDATA_HPP
