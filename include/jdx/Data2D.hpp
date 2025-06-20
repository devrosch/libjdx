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
#ifndef JDX_DATA2D_HPP
#define JDX_DATA2D_HPP

#include "io/TextReader.hpp"
#include "jdx/DataLdr.hpp"

#include <vector>

namespace libjdx::jdx
{
/**
 * @brief A JCAMP-DX 2D data record. Base for "XYPOINTS", XYDATA", "RADATA", and
 * "DATA TABLE" LDRs.
 */
class Data2D : public DataLdr
{
public:
protected:
    enum class VariableList
    {
        /** (X++(Y..Y)) */
        XppYY,

        /** (R++(A..A)) */
        RppAA,

        /** (X++(R..R)) */
        XppRR,

        /** (X++(I..I)) */
        XppII,

        /** (T2++(R..R)) */
        T2ppRR,

        /** (T2++(I..I)) */
        T2ppII,

        /** (F2++(Y..Y)) */
        F2ppYY,

        /** (XY..XY) */
        XYXY,

        /** (XR..XR) */
        XRXR,

        /** (XI..XI) */
        XIXI,
    };

    /**
     * @brief Constructs Array2DData from first line value and reader.
     * @param label The label of the first line of the record, i.e. "XYDATA",
     * "RADATA", or "DATATABLE".
     * @param variableList The value of the first line of the record
     * representing the structure of the data, e.g. "(X++(Y..Y))".
     * @param reader Text reader with JCAMP-DX data. The reader position is
     * assumed to be at the start of the first line (the line containing
     * XYDATA or RADATA) of the record. The reader is expected to exist for the
     * lifetime of this object.
     */
    Data2D(std::string label, std::string variableList, io::TextReader& reader);

    /**
     * @brief Parses the equally x spaced xy data (i.e. "X++(Y..Y)", "R++(A..A)"
     * "X++(R..R)", or "X++(I..I)") from an XYDATA or RADATA block.
     * @param label The label of the first line of the record, i.e. "XYDATA" or
     * "RADATA".
     * @param reader Text reader with JCAMP-DX data. The reader position is
     * assumed to be at the start of the second line (the line following the
     * line containing the label) of the record. The reader is expected to exist
     * for the lifetime of this object.
     * @param firstX The first X value.
     * @param lastX The last X value.
     * @param yFactor The factor by which to multiply raw y values to arrive at
     * the actual value.
     * @param nPoints The number of xy pairs in this record.
     * @return Pairs of xy data. Invalid values ("?") will be represented by
     * std::numeric_limits<T>::quiet_NaN.
     *
     * Note: XFACTOR is not required for parsing as the x values are determined
     * by FIRSTX, LASTX and NPOINTS.
     */
    std::vector<std::pair<double, double>> parseXppYYData(
        const std::string& label, io::TextReader& reader, double firstX,
        double lastX, double yFactor, uint64_t nPoints) const;

    /**
     * @brief Parses the xy data pairs (i.e. "(XY..XY)" or "(RA..RA)") from a
     * "##XYDATA=" or "##RADATA=" block.
     * @param label The label of the first line of the record, i.e. "XYDATA" or
     * "RADATA".
     * @param reader Text reader with JCAMP-DX data. The reader position is
     * assumed to be at the start of the second line (the line following the
     * line containing the label) of the record. The reader is expected to exist
     * for the lifetime of this object.
     * @param xFactor The factor by which to multiply raw x values to arrive at
     * the actual value.
     * @param yFactor The factor by which to multiply raw y values to arrive at
     * the actual value.
     * @param nPoints The number of xy pairs in this record.
     * @return Pairs of xy data. Invalid y values ("?") will be represented by
     * std::numeric_limits<T>::quiet_NaN.
     */
    std::vector<std::pair<double, double>> parseXyXyData(
        const std::string& label, io::TextReader& reader, double xFactor,
        double yFactor, std::optional<size_t> nPoints) const;
};
} // namespace libjdx::jdx

#endif // JDX_DATA2D_HPP
