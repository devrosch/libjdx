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
#include "jdx/JdxParser.hpp"
#include "jdx/XyData.hpp"

#include "catch2/catch.hpp"

#include <fstream>
#include <sstream>

TEST_CASE("parses XyData from SimpleFile.jdx sample file",
    "[IntegrationTest][XyData]")
{
    const std::string path{"resources/SimpleFile.jdx"};
    auto istream = std::make_unique<std::ifstream>(path);

    auto block = sciformats::jdx::JdxParser::parse(std::move(istream));

    REQUIRE(block.getXyData());

    auto xyData = block.getXyData().value();
    auto data = xyData.getData();
    REQUIRE(data.size() == 2);
}

TEST_CASE("parses Bruker specific sample file", "[IntegrationTest]")
{
    const std::string path{"resources/Bruker_specific.jdx"};
    auto istream = std::make_unique<std::ifstream>(path);

    auto block = sciformats::jdx::JdxParser::parse(std::move(istream));

    const auto& brukerParameterSections = block.getBrukerSpecificParameters();

    REQUIRE(brukerParameterSections.size() == 2);
    REQUIRE(brukerParameterSections.at(0).getContent().size() == 4);
    REQUIRE(brukerParameterSections.at(1).getContent().size() == 3);
}

TEST_CASE("parses Bruker specific sample file with relax sections",
    "[IntegrationTest]")
{
    const std::string path{"resources/Bruker_specific_relax.jdx"};
    auto istream = std::make_unique<std::ifstream>(path);

    auto block = sciformats::jdx::JdxParser::parse(std::move(istream));

    const auto& brukerParameterSections = block.getBrukerSpecificParameters();

    REQUIRE(brukerParameterSections.size() == 2);
    REQUIRE(brukerParameterSections.at(0).getContent().size() == 4);
    REQUIRE(brukerParameterSections.at(1).getContent().size() == 3);

    const auto& brukerRelaxSections = block.getBrukerRelaxSections();

    REQUIRE(brukerRelaxSections.size() == 2);
}
