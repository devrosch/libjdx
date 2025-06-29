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
#include "jdx/BrukerSpecificParameters.hpp"

#include "catch2/catch.hpp"

#include <fstream>
#include <sstream>

TEST_CASE("parses regular Bruker parameters section",
    "[BrukerSpecificParameters][quirk]")
{
    auto nextLine = std::optional<std::string>{"$$ Bruker specific parameters"};
    // clang-format off
    std::string input{
        "$$ --------------------------\n"
        "##$DU= <C:/>\n"
        "##$NAME= <Jul11-2023>\n"
        "##$AQSEQ= 0\n"
        "##$AQ_mod= 3\n"
        "$$ Bruker specific parameters for F1\n"
        "$$ ---------------------------------\n"};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    libjdx::jdx::BrukerSpecificParameters brukerParameterSection{
        reader, nextLine};

    REQUIRE("Bruker specific parameters" == brukerParameterSection.getName());
    auto content = brukerParameterSection.getContent();
    REQUIRE(4 == content.size());
    REQUIRE("$DU" == content.at(0).getLabel());
    REQUIRE("<C:/>" == content.at(0).getValue());
    REQUIRE("$NAME" == content.at(1).getLabel());
    REQUIRE("<Jul11-2023>" == content.at(1).getValue());
    REQUIRE("$AQSEQ" == content.at(2).getLabel());
    REQUIRE("0" == content.at(2).getValue());
    REQUIRE("$AQMOD" == content.at(3).getLabel());
    REQUIRE("3" == content.at(3).getValue());
}

TEST_CASE("parses Bruker parameters section for F1",
    "[BrukerSpecificParameters][quirk]")
{
    auto nextLine
        = std::optional<std::string>{"$$ Bruker specific parameters for F1"};
    // clang-format off
    std::string input{
        "$$ ---------------------------------\n"
        "##$AMP= (0..31)\n"
        "100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 \n"
        "100 100 100 100 100 100 100 100 100 100 100 100 100 100 \n"
        "##$AQSEQ= 0\n"
        "##$AQ_mod= 2\n"
        "$$ End of Bruker specific parameters\n"
        "$$ ---------------------------------\n"};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    libjdx::jdx::BrukerSpecificParameters brukerParameterSection{
        reader, nextLine};

    REQUIRE("Bruker specific parameters for F1"
            == brukerParameterSection.getName());
    auto content = brukerParameterSection.getContent();
    REQUIRE(3 == content.size());
    REQUIRE("$AMP" == content.at(0).getLabel());
    REQUIRE("(0..31)\n"
            "100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 100 "
            "100 100 \n"
            "100 100 100 100 100 100 100 100 100 100 100 100 100 100 "
            == content.at(0).getValue());
    REQUIRE("$AQSEQ" == content.at(1).getLabel());
    REQUIRE("0" == content.at(1).getValue());
    REQUIRE("$AQMOD" == content.at(2).getLabel());
    REQUIRE("2" == content.at(2).getValue());
}

TEST_CASE("fails on premature end of Bruker parameters section",
    "[BrukerSpecificParameters][quirk]")
{
    auto nextLine = std::optional<std::string>{"$$ Bruker specific parameters"};
    // clang-format off
    std::string input{
        "$$ --------------------------\n"
        "##$DU= <C:/>\n"};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    REQUIRE_THROWS_WITH(libjdx::jdx::BrukerSpecificParameters(reader, nextLine),
        Catch::Matchers::Contains("unexpected", Catch::CaseSensitive::No));
}

TEST_CASE("fails on missing dashes after start of Bruker parameters section",
    "[BrukerSpecificParameters][quirk]")
{
    auto nextLine = std::optional<std::string>{"$$ Bruker specific parameters"};
    // clang-format off
    std::string input{
        "##$DU= <C:/>\n"
        "##$NAME= <Jul11-2023>\n"
        "$$ Bruker specific parameters for F1\n"
        "$$ ---------------------------------\n"};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    REQUIRE_THROWS_WITH(libjdx::jdx::BrukerSpecificParameters(reader, nextLine),
        Catch::Matchers::Contains("illegal", Catch::CaseSensitive::No));
}

TEST_CASE("fails on missing dashes after end of Bruker parameters section",
    "[BrukerSpecificParameters][quirk]")
{
    auto nextLine
        = std::optional<std::string>{"$$ Bruker specific parameters for F1"};
    // clang-format off
    std::string input{
        "$$ ---------------------------------\n"
        "##$AQSEQ= 0\n"
        "$$ End of Bruker specific parameters\n"};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    REQUIRE_THROWS_WITH(libjdx::jdx::BrukerSpecificParameters(reader, nextLine),
        Catch::Matchers::Contains("illegal", Catch::CaseSensitive::No));
}

TEST_CASE("fails on illegal start of Bruker parameters section",
    "[BrukerSpecificParameters][quirk]")
{
    auto nextLine
        = std::optional<std::string>{"$$ Not Bruker specific parameters"};
    // clang-format off
    std::string input{
        "$$ ---------------------------------\n"
        "##$AQSEQ= 0\n"
        "$$ End of Bruker specific parameters\n"
        "$$ ---------------------------------\n"};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    REQUIRE_THROWS_WITH(libjdx::jdx::BrukerSpecificParameters(reader, nextLine),
        Catch::Matchers::Contains("illegal", Catch::CaseSensitive::No));
}

TEST_CASE("fails on premature end of stream in Bruker parameters section",
    "[BrukerSpecificParameters][quirk]")
{
    auto nextLine = std::optional<std::string>{"$$ Bruker specific parameters"};
    // clang-format off
    std::string input{};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    REQUIRE_THROWS_WITH(libjdx::jdx::BrukerSpecificParameters(reader, nextLine),
        Catch::Matchers::Contains("illegal", Catch::CaseSensitive::No));
}
