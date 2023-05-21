#include "jdx/RaData.hpp"
#include "jdx/ParseException.hpp"
#include "jdx/RaParameters.hpp"
#include "util/DataParser.hpp"
#include "util/LdrUtils.hpp"

#include <limits>

sciformats::jdx::RaData::RaData(const std::string& label,
    const std::string& variableList, const std::vector<StringLdr>& ldrs,
    io::TextReader& reader, std::optional<std::string>& nextLine)
    : Data2D(label, variableList, reader)
{
    validateInput(label, variableList, s_raDataLabel,
        std::vector<std::string>{s_raDataVariableList});
    m_parameters = parseParameters(ldrs);
    util::skipToNextLdr(reader, nextLine, true);
}

const sciformats::jdx::RaParameters&
sciformats::jdx::RaData::getParameters() const
{
    return m_parameters;
}

std::vector<std::pair<double, double>> sciformats::jdx::RaData::getData()
{
    return Data2D::parseXppYYData(getLabel(), getReader(), m_parameters.firstR,
        m_parameters.lastR, m_parameters.rFactor, m_parameters.nPoints);
}

sciformats::jdx::RaParameters sciformats::jdx::RaData::parseParameters(
    const std::vector<StringLdr>& ldrs)
{
    // required
    // string
    auto rUnits = util::parseLdrValue<std::string>(ldrs, "RUNITS");
    auto aUnits = util::parseLdrValue<std::string>(ldrs, "AUNITS");
    // double
    auto firstR = util::parseLdrValue<double>(ldrs, "FIRSTR");
    auto lastR = util::parseLdrValue<double>(ldrs, "LASTR");
    auto rFactor = util::parseLdrValue<double>(ldrs, "RFACTOR");
    auto aFactor = util::parseLdrValue<double>(ldrs, "AFACTOR");
    // uint64_t
    auto nPoints = util::parseLdrValue<uint64_t>(ldrs, "NPOINTS");
    // optional
    // double
    auto firstA = util::parseLdrValue<double>(ldrs, "FIRSTA");
    auto maxA
        = util::parseLdrValue<double>(ldrs, "MAXA"); // required, according to standard
    auto minA
        = util::parseLdrValue<double>(ldrs, "MINA"); // required, according to standard
    auto resolution = util::parseLdrValue<double>(ldrs, "RESOLUTION");
    auto deltaR = util::parseLdrValue<double>(ldrs, "DELTAR");
    auto zdp = util::parseLdrValue<double>(ldrs, "ZDP");
    // string
    auto alias = util::parseLdrValue<std::string>(ldrs, "ALIAS");

    std::string missing{};
    missing += rUnits.has_value() ? "" : " RUNITS";
    missing += aUnits.has_value() ? "" : " AUNITS";
    missing += firstR.has_value() ? "" : " FIRSTR";
    missing += lastR.has_value() ? "" : " LASTR";
    missing += rFactor.has_value() ? "" : " RFACTOR";
    missing += aFactor.has_value() ? "" : " AFACTOR";
    missing += nPoints.has_value() ? "" : " NPOINTS";
    if (!missing.empty())
    {
        throw ParseException(
            "Required LDR(s) missing for RADATA: {" + missing + " }");
    }

    RaParameters parameters {
        rUnits.value(),
        aUnits.value(),
        firstR.value(),
        lastR.value(),
        maxA,
        minA,
        rFactor.value(),
        aFactor.value(),
        nPoints.value(),
        firstA,
        resolution,
        deltaR,
        zdp,
        alias,
    };

    return parameters;
}
