#include "jdx/XyBase.hpp"
#include "jdx/ParseException.hpp"
#include "util/DataParser.hpp"
#include "util/LdrUtils.hpp"

#include <limits>

sciformats::jdx::XyBase::XyBase(const std::string& label,
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

const sciformats::jdx::XyParameters&
sciformats::jdx::XyBase::getParameters() const
{
    return m_parameters;
}

std::vector<std::pair<double, double>>
sciformats::jdx::XyBase::getXppYYData() const
{
    return Data2D::parseXppYYData(getLabel(), getReader(), m_parameters.firstX,
        m_parameters.lastX, m_parameters.yFactor, m_parameters.nPoints);
}

std::vector<std::pair<double, double>>
sciformats::jdx::XyBase::getXYXYData() const
{
    return Data2D::parseXyXyData(getLabel(), getReader(), m_parameters.xFactor,
        m_parameters.yFactor, m_parameters.nPoints);
}

sciformats::jdx::XyParameters sciformats::jdx::XyBase::parseParameters(
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

    XyParameters parameters
    {
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
