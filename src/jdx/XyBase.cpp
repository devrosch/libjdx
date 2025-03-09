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
#include "jdx/XyBase.hpp"
#include "jdx/ParseException.hpp"
#include "util/DataParser.hpp"
#include "util/LdrUtils.hpp"

#include <limits>

libjdx::jdx::XyBase::XyBase(const std::string& label,
    const std::string& variableList, const std::vector<StringLdr>& ldrs,
    const std::string& expectedLabel,
    const std::vector<std::string>& expectedVariableLists,
    io::TextReader& reader, std::optional<std::string>& nextLine)
    : Data2D{label, variableList, reader}
{
    validateInput(
        getLabel(), getVariableList(), expectedLabel, expectedVariableLists);
    m_parameters = parseParameters(ldrs);
    util::skipToNextLdr(reader, nextLine, true);
}

const libjdx::jdx::XyParameters& libjdx::jdx::XyBase::getParameters() const
{
    return m_parameters;
}

std::vector<std::pair<double, double>> libjdx::jdx::XyBase::getXppYYData() const
{
    return Data2D::parseXppYYData(getLabel(), getReader(), m_parameters.firstX,
        m_parameters.lastX, m_parameters.yFactor, m_parameters.nPoints);
}

std::vector<std::pair<double, double>> libjdx::jdx::XyBase::getXYXYData() const
{
    return Data2D::parseXyXyData(getLabel(), getReader(), m_parameters.xFactor,
        m_parameters.yFactor, m_parameters.nPoints);
}

libjdx::jdx::XyParameters libjdx::jdx::XyBase::parseParameters(
    const std::vector<StringLdr>& ldrs)
{
    // required
    // string
    auto xUnits = util::parseLdrValue<std::string>(ldrs, "XUNITS");
    auto yUnits = util::parseLdrValue<std::string>(ldrs, "YUNITS");
    // double
    auto firstX = util::parseLdrValue<double>(ldrs, "FIRSTX");
    auto lastX = util::parseLdrValue<double>(ldrs, "LASTX");
    auto xFactor = util::parseLdrValue<double>(ldrs, "XFACTOR");
    auto yFactor = util::parseLdrValue<double>(ldrs, "YFACTOR");
    // uint64_t
    auto nPoints = util::parseLdrValue<uint64_t>(ldrs, "NPOINTS");
    // optional
    // double
    auto firstY = util::parseLdrValue<double>(ldrs, "FIRSTY");
    auto maxX = util::parseLdrValue<double>(ldrs, "MAXX");
    auto minX = util::parseLdrValue<double>(ldrs, "MINX");
    auto maxY = util::parseLdrValue<double>(ldrs, "MAXY");
    auto minY = util::parseLdrValue<double>(ldrs, "MINY");
    auto resolution = util::parseLdrValue<double>(ldrs, "RESOLUTION");
    auto deltaX = util::parseLdrValue<double>(ldrs, "DELTAX");

    std::string missing{};
    missing += xUnits.has_value() ? "" : " XUNITS";
    missing += yUnits.has_value() ? "" : " YUNITS";
    missing += firstX.has_value() ? "" : " FIRSTX";
    missing += lastX.has_value() ? "" : " LASTX";
    missing += xFactor.has_value() ? "" : " XFACTOR";
    missing += yFactor.has_value() ? "" : " YFACTOR";
    missing += nPoints.has_value() ? "" : " NPOINTS";
    if (!missing.empty())
    {
        throw ParseException(
            "Required LDR(s) missing for XYDATA: {" + missing + " }");
    }

    XyParameters parameters{
        xUnits.value(),
        yUnits.value(),
        firstX.value(),
        lastX.value(),
        maxX,
        minX,
        maxY,
        minY,
        xFactor.value(),
        yFactor.value(),
        nPoints.value(),
        firstY,
        resolution,
        deltaX,
    };

    return parameters;
}
