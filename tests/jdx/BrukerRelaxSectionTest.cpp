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
#include "jdx/BrukerRelaxSection.hpp"

#include "catch2/catch.hpp"

#include <fstream>
#include <sstream>

TEST_CASE("parses Bruker $RELAX section with arbitrary content",
    "[BrukerRelaxSection][quirk]")
{
    const auto* label = "$RELAX";
    const auto* value = "";
    auto nextLine = std::optional<std::string>{"##$RELAX=\n"};
    // clang-format off
    std::string input{
        "##$BRUKER FILE EXP=file_name_1\n"
        "$$ 1.0\n"
        "$$0.0 1.0 2.0\n"
        "$$    123   \n"
        "##$RELAX=\n"};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    libjdx::jdx::BrukerRelaxSection brukerRelaxSection{
        label, value, reader, nextLine};

    REQUIRE("file_name_1" == brukerRelaxSection.getName());
    REQUIRE("1.0\n0.0 1.0 2.0\n   123   \n" == brukerRelaxSection.getContent());
}

TEST_CASE("parses Bruker $RELAX section with JCAMP-DX like content",
    "[BrukerRelaxSection][quirk]")
{
    const auto* label = "$RELAX";
    const auto* value = "";
    auto nextLine = std::optional<std::string>{"##$RELAX=\n"};
    // clang-format off
    std::string input{
        "##$BRUKER FILE EXP=file_name_2\n"
        "$$ ##TITLE= Parameter file\n"
        "$$ ##JCAMPDX= 5.0\n"
        "$$ $$ c:/nmr/data/somepath\n"
        "$$ ##$BLKPA= (0..15)\n"
        "3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 \n"
        "$$ ##$BLKTR= (0..15)\n"
        "3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 \n"
        "$$ ##$DE1= 2\n"
        "$$ ##END=\n"
        "##$RELAX=\n"};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    libjdx::jdx::BrukerRelaxSection brukerRelaxSection{
        label, value, reader, nextLine};

    REQUIRE("file_name_2" == brukerRelaxSection.getName());
    REQUIRE("##TITLE= Parameter file\n"
            "##JCAMPDX= 5.0\n"
            "$$ c:/nmr/data/somepath\n"
            "##$BLKPA= (0..15)\n"
            "3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 \n"
            "##$BLKTR= (0..15)\n"
            "3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 \n"
            "##$DE1= 2\n"
            "##END=\n"
            == brukerRelaxSection.getContent());
}

TEST_CASE("parses empty Bruker $RELAX section", "[BrukerRelaxSection][quirk]")
{
    const auto* label = "$RELAX";
    const auto* value = "";
    auto nextLine = std::optional<std::string>{"##$RELAX=\n"};
    // clang-format off
    std::string input{
        "$$ Bruker specific parameters\n"
        "$$ --------------------------\n"};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    libjdx::jdx::BrukerRelaxSection brukerRelaxSection{
        label, value, reader, nextLine};

    REQUIRE(brukerRelaxSection.getName().empty());
    REQUIRE(brukerRelaxSection.getContent().empty());
}

TEST_CASE("fails for illegal parameters for Bruker $RELAX section",
    "[BrukerRelaxSection][quirk]")
{
    const auto* label = "TITLE";
    const auto* value = "abc";
    auto nextLine = std::optional<std::string>{"##TITLE= abc\n"};
    // clang-format off
    std::string input{"xxx\n"};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    REQUIRE_THROWS_WITH(
        libjdx::jdx::BrukerRelaxSection(label, value, reader, nextLine),
        Catch::Matchers::Contains("illegal", Catch::CaseSensitive::No));
}

TEST_CASE("fails for premature end of Bruker $RELAX section",
    "[BrukerRelaxSection][quirk]")
{
    const auto* label = "$RELAX";
    const auto* value = "";
    auto nextLine = std::optional<std::string>{"##$RELAX=\n"};
    // clang-format off
    std::string input{};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    REQUIRE_THROWS_WITH(
        libjdx::jdx::BrukerRelaxSection(label, value, reader, nextLine),
        Catch::Matchers::Contains("premature", Catch::CaseSensitive::No));
}

TEST_CASE("fails for illegal start of Bruker $RELAX section",
    "[BrukerRelaxSection][quirk]")
{
    const auto* label = "$RELAX";
    const auto* value = "";
    auto nextLine = std::optional<std::string>{"##$RELAX=\n"};
    // clang-format off
    std::string input{
        "##SOME_LABEL= abc\n"};
    // clang-format on

    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    REQUIRE_THROWS_WITH(
        libjdx::jdx::BrukerRelaxSection(label, value, reader, nextLine),
        Catch::Matchers::Contains("illegal", Catch::CaseSensitive::No)
            && Catch::Matchers::Contains(
                "not followed by", Catch::CaseSensitive::No));
}
