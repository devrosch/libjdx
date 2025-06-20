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
#include "jdx/RaData.hpp"
#include "jdx/StringLdr.hpp"

#include "catch2/catch.hpp"

#include <sstream>

TEST_CASE("parses AFFN RA data with minimum required parameters", "[RaData]")
{
    // "##RADATA= (R++(A..A))\r\n"
    const auto* label = "RADATA";
    const auto* variables = "(R++(A..A))";
    std::string input{"0, 10.0\r\n"
                      "1, 11.0\r\n"
                      "2, 12.0\r\n"
                      "##END="};
    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    std::vector<libjdx::jdx::StringLdr> ldrs;
    ldrs.emplace_back("RUNITS", "MICROMETERS");
    ldrs.emplace_back("AUNITS", "ARBITRARY UNITS");
    ldrs.emplace_back("FIRSTR", "0");
    ldrs.emplace_back("LASTR", "2");
    ldrs.emplace_back("RFACTOR", "1.0");
    ldrs.emplace_back("AFACTOR", "1.0");
    ldrs.emplace_back("NPOINTS", "3");
    auto nextLine = std::optional<std::string>{};
    auto raDataRecord
        = libjdx::jdx::RaData(label, variables, ldrs, reader, nextLine);

    REQUIRE("(R++(A..A))" == raDataRecord.getVariableList());

    auto raData = raDataRecord.getData();
    REQUIRE(3 == raData.size());
    REQUIRE(0 == Approx(raData.at(0).first));
    REQUIRE(10.0 == Approx(raData.at(0).second));
    REQUIRE(1 == Approx(raData.at(1).first));
    REQUIRE(11.0 == Approx(raData.at(1).second));
    REQUIRE(2 == Approx(raData.at(2).first));
    REQUIRE(12.0 == Approx(raData.at(2).second));

    auto params = raDataRecord.getParameters();
    REQUIRE("MICROMETERS" == params.rUnits);
    REQUIRE("ARBITRARY UNITS" == params.aUnits);
    REQUIRE(0.0 == Approx(params.firstR));
    REQUIRE(2.0 == Approx(params.lastR));
    REQUIRE(1.0 == Approx(params.rFactor));
    REQUIRE(1.0 == Approx(params.aFactor));
    REQUIRE(3 == params.nPoints);
    REQUIRE_FALSE(params.firstA.has_value());
    REQUIRE_FALSE(params.resolution.has_value());
    REQUIRE_FALSE(params.deltaR.has_value());
    REQUIRE_FALSE(params.zdp.has_value());
    REQUIRE_FALSE(params.alias.has_value());
}

TEST_CASE("parses AFFN RA data with all parameters", "[RaData]")
{
    // "##RADATA= (R++(A..A))\r\n"
    const auto* label = "RADATA";
    const auto* variables = "(R++(A..A))";
    std::string input{"0, 10.0\r\n"
                      "1, 11.0\r\n"
                      "2, 12.0\r\n"
                      "##END="};
    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    std::vector<libjdx::jdx::StringLdr> ldrs;
    ldrs.emplace_back("RUNITS", "MICROMETERS");
    ldrs.emplace_back("AUNITS", "ARBITRARY UNITS");
    ldrs.emplace_back("FIRSTR", "0");
    ldrs.emplace_back("LASTR", "2");
    ldrs.emplace_back("RFACTOR", "1.0");
    ldrs.emplace_back("AFACTOR", "1.0");
    ldrs.emplace_back("NPOINTS", "3");
    ldrs.emplace_back("FIRSTA", "10.0");
    ldrs.emplace_back("MAXA", "12.0");
    ldrs.emplace_back("MINA", "10.0");
    ldrs.emplace_back("RESOLUTION", "2.0");
    ldrs.emplace_back("DELTAR", "1.0");
    ldrs.emplace_back("ZDP", "1");
    ldrs.emplace_back("ALIAS", "1/2");
    auto nextLine = std::optional<std::string>{};
    auto raDataRecord
        = libjdx::jdx::RaData(label, variables, ldrs, reader, nextLine);

    auto raData = raDataRecord.getData();

    REQUIRE(3 == raData.size());
    REQUIRE(0 == Approx(raData.at(0).first));
    REQUIRE(10.0 == Approx(raData.at(0).second));
    REQUIRE(1 == Approx(raData.at(1).first));
    REQUIRE(11.0 == Approx(raData.at(1).second));
    REQUIRE(2 == Approx(raData.at(2).first));
    REQUIRE(12.0 == Approx(raData.at(2).second));

    auto params = raDataRecord.getParameters();
    REQUIRE("MICROMETERS" == params.rUnits);
    REQUIRE("ARBITRARY UNITS" == params.aUnits);
    REQUIRE(0.0 == Approx(params.firstR));
    REQUIRE(2.0 == Approx(params.lastR));
    REQUIRE(1.0 == Approx(params.rFactor));
    REQUIRE(1.0 == Approx(params.aFactor));
    REQUIRE(3 == params.nPoints);
    REQUIRE(10.0 == params.firstA.value());
    REQUIRE(12.0 == params.maxA.value());
    REQUIRE(10.0 == params.minA.value());
    REQUIRE(2.0 == params.resolution.value());
    REQUIRE(1.0 == params.deltaR.value());
    REQUIRE(1.0 == params.zdp.value());
    REQUIRE("1/2" == params.alias.value());
}

TEST_CASE("accepts blank values for optional RA data parameters", "[RaData]")
{
    // "##RADATA= (R++(A..A))\r\n"
    const auto* label = "RADATA";
    const auto* variables = "(R++(A..A))";
    std::string input{"0, 10.0\r\n"
                      "1, 11.0\r\n"
                      "2, 12.0\r\n"
                      "##END="};
    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    std::vector<libjdx::jdx::StringLdr> ldrs;
    ldrs.emplace_back("RUNITS", "MICROMETERS");
    ldrs.emplace_back("AUNITS", "ARBITRARY UNITS");
    ldrs.emplace_back("FIRSTR", "0");
    ldrs.emplace_back("LASTR", "2");
    ldrs.emplace_back("RFACTOR", "1.0");
    ldrs.emplace_back("AFACTOR", "1.0");
    ldrs.emplace_back("NPOINTS", "3");
    ldrs.emplace_back("FIRSTA", "");
    ldrs.emplace_back("MAXA", "");
    ldrs.emplace_back("MINA", "");
    ldrs.emplace_back("RESOLUTION", "");
    ldrs.emplace_back("DELTAR", "");
    ldrs.emplace_back("ZDP", "");
    ldrs.emplace_back("ALIAS", "");

    auto nextLine = std::optional<std::string>{};
    auto raDataRecord
        = libjdx::jdx::RaData(label, variables, ldrs, reader, nextLine);
    auto raData = raDataRecord.getData();
    auto params = raDataRecord.getParameters();

    REQUIRE("MICROMETERS" == params.rUnits);
    REQUIRE("ARBITRARY UNITS" == params.aUnits);
    REQUIRE(0.0 == Approx(params.firstR));
    REQUIRE(2.0 == Approx(params.lastR));
    REQUIRE(1.0 == Approx(params.rFactor));
    REQUIRE(1.0 == Approx(params.aFactor));
    REQUIRE(3 == params.nPoints);
    REQUIRE_FALSE(params.firstA.has_value());
    REQUIRE_FALSE(params.maxA.has_value());
    REQUIRE_FALSE(params.minA.has_value());
    REQUIRE_FALSE(params.resolution.has_value());
    REQUIRE_FALSE(params.deltaR.has_value());
    REQUIRE_FALSE(params.zdp.has_value());
    REQUIRE(params.alias.value().empty());
}

TEST_CASE("detects mismatching variables list for RADATA", "[RaData]")
{
    // "##RADATA= (R++(A..A))\r\n"
    const auto* label = "RADATA";
    const auto* variables = "(R++(A..A))";
    std::string input{"0, 10.0\r\n"
                      "##END="};
    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    std::vector<libjdx::jdx::StringLdr> ldrs;
    ldrs.emplace_back("RUNITS", "MICROMETERS");
    ldrs.emplace_back("AUNITS", "ARBITRARY UNITS");
    ldrs.emplace_back("FIRSTR", "0");
    ldrs.emplace_back("LASTR", "0");
    ldrs.emplace_back("RFACTOR", "1.0");
    ldrs.emplace_back("AFACTOR", "1.0");
    // NPOINTS missing

    auto nextLine = std::optional<std::string>{};

    REQUIRE_THROWS_WITH(
        libjdx::jdx::RaData(label, variables, ldrs, reader, nextLine),
        Catch::Matchers::Contains("missing", Catch::CaseSensitive::No)
            && Catch::Matchers::Contains("NPOINTS", Catch::CaseSensitive::No));
}
