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
#include "util/LdrUtils.hpp"

#include "catch2/catch.hpp"

#include <iostream>
#include <sstream>
#include <string>

TEST_CASE("recognizes regular LDR start", "[util][isLdrStart]")
{
    std::string input{"##TITLE= abc"};

    REQUIRE(true == sciformats::jdx::util::isLdrStart(input));
}

TEST_CASE(
    "recognizes LDR start with leading white spaces", "[util][isLdrStart]")
{
    std::string input{"\t\n\v\f\r ##TITLE= abc"};

    REQUIRE(true == sciformats::jdx::util::isLdrStart(input));
}

TEST_CASE("recognizes LDR start with trailing line break characters",
    "[util][isLdrStart]")
{
    std::string inputLf{"##TITLE= abc\n"};
    std::string inputCr{"##TITLE= abc\r"};
    std::string inputCrLf{"##TITLE= abc\r\n"};
    std::string inputCrLfLsPs{"##TITLE= abc\r\n\u2028\u2029"};

    REQUIRE(sciformats::jdx::util::isLdrStart(inputLf));
    REQUIRE(sciformats::jdx::util::isLdrStart(inputCr));
    REQUIRE(sciformats::jdx::util::isLdrStart(inputCrLf));
    REQUIRE(sciformats::jdx::util::isLdrStart(inputCrLfLsPs));
}

TEST_CASE(
    "rejects string if non whitespace preceeds LDR start", "[util][isLdrStart]")
{
    std::string inputLf{"xyz ##TITLE= abc"};

    REQUIRE_FALSE(sciformats::jdx::util::isLdrStart(inputLf));
}

TEST_CASE("recognizes LDR start with labels containing special characters and "
          "numbers",
    "[util][isLdrStart]")
{
    std::string input{"##.N_A/M2E$= abc"};

    REQUIRE(true == sciformats::jdx::util::isLdrStart(input));
}

TEST_CASE("rejects non LDR start", "[util][isLdrStart]")
{
    std::string input{"#NAME= ##NOT_LDR=abc"};

    REQUIRE(false == sciformats::jdx::util::isLdrStart(input));
}

TEST_CASE("normalize LDR start removes \" -/_\" from label",
    "[util][normalizeLdrLabel]")
{
    std::string input{"##A B-C/D_E= abc"};
    std::string expect{"##ABCDE= abc"};

    std::string actual = sciformats::jdx::util::normalizeLdrStart(input);

    REQUIRE(expect == actual);
}

TEST_CASE("normalize LDR start leaves normalized label intact",
    "[util][normalizeLdrLabel]")
{
    std::string input{"##ABCDE= abc"};
    std::string expect{"##ABCDE= abc"};

    std::string actual = sciformats::jdx::util::normalizeLdrStart(input);

    REQUIRE(expect == actual);
}

TEST_CASE("normalize LDR start removes leading white spaces",
    "[util][normalizeLdrLabel]")
{
    std::string input{"\t\n\v\f\r ##ABCDE= abc"};
    std::string expect{"##ABCDE= abc"};

    std::string actual = sciformats::jdx::util::normalizeLdrStart(input);

    REQUIRE(expect == actual);
}

TEST_CASE("normalize LDR start turns (only) ASCII letters to upper case",
    "[util][normalizeLdrLabel]")
{
    // label: abcdeäöüÄÖÜ in ISO-8859-1 encoding
    std::string input{"##abcde\xE4\xF6\xFC\xC4\xD6\xDC= abc"};
    std::string expect{"##ABCDE\xE4\xF6\xFC\xC4\xD6\xDC= abc"};

    std::string actual = sciformats::jdx::util::normalizeLdrStart(input);

    REQUIRE(expect == actual);
}

TEST_CASE(
    "rejects missing double hashes in LDR start", "[util][normalizeLdrLabel]")
{
    std::string input{"#LABEL= abc"};

    REQUIRE_THROWS(sciformats::jdx::util::normalizeLdrStart(input));
}

TEST_CASE("rejects missing equals in LDR start", "[util][normalizeLdrLabel]")
{
    std::string input{"##LABEL abc"};

    REQUIRE_THROWS(sciformats::jdx::util::normalizeLdrStart(input));
}

TEST_CASE("tokenizes regular LDR start", "[util][parseLdrStart]")
{
    std::string input{"##LABEL=abc"};

    auto [label, value] = sciformats::jdx::util::parseLdrStart(input);

    REQUIRE("LABEL" == label);
    REQUIRE("abc" == value);
}

TEST_CASE("tokenizes LDR start with missing value", "[util][parseLdrStart]")
{
    std::string input{"##LABEL="};

    auto [label, value] = sciformats::jdx::util::parseLdrStart(input);

    REQUIRE("LABEL" == label);
    REQUIRE(value.empty());
}

TEST_CASE("removes (only) first leading space LDR start value",
    "[util][parseLdrStart]")
{
    std::string input{"##LABEL=  abc"};

    auto [label, value] = sciformats::jdx::util::parseLdrStart(input);

    REQUIRE("LABEL" == label);
    REQUIRE(" abc" == value);
}

TEST_CASE("normalizes LDR start label", "[util][parseLdrStart]")
{
    std::string input{"\t\n\v\f\r ##abcde\xE4\xF6\xFC\xC4\xD6\xDC="};

    auto label = sciformats::jdx::util::parseLdrStart(input).first;

    REQUIRE("ABCDE\xE4\xF6\xFC\xC4\xD6\xDC" == label);
}

TEST_CASE("rejects malformed LDR start (missing hash)", "[util][parseLdrStart]")
{
    std::string input{"#LABEL="};

    REQUIRE_THROWS(sciformats::jdx::util::parseLdrStart(input));
}

TEST_CASE(
    "rejects malformed LDR start (missing equals)", "[util][parseLdrStart]")
{
    std::string input{"##LABEL"};

    REQUIRE_THROWS(sciformats::jdx::util::parseLdrStart(input));
}

TEST_CASE("strips line comment", "[util][stripLineComment]")
{
    std::string input{"line start $$ comment"};
    auto [content, comment] = sciformats::jdx::util::stripLineComment(input);

    REQUIRE("line start " == content);
    REQUIRE(comment.has_value());
    REQUIRE(" comment" == comment);
}

TEST_CASE("indicates missing comment with nullopt", "[util][stripLineComment]")
{
    std::string input{"line content"};
    auto [content, comment] = sciformats::jdx::util::stripLineComment(input);

    REQUIRE("line content" == content);
    REQUIRE(!comment.has_value());
}

TEST_CASE("indicates empty content if whole line is comment",
    "[util][stripLineComment]")
{
    std::string input{"$$line comment"};
    auto [content, comment] = sciformats::jdx::util::stripLineComment(input);

    REQUIRE(content.empty());
    REQUIRE(comment.has_value());
    REQUIRE("line comment" == comment);
}

TEST_CASE(
    "indicates empty comment with empty string", "[util][stripLineComment]")
{
    std::string input{"line content$$"};
    auto [content, comment] = sciformats::jdx::util::stripLineComment(input);

    REQUIRE(!content.empty());
    REQUIRE("line content" == content);
    REQUIRE(comment.has_value());
    REQUIRE(comment.value().empty());
}

TEST_CASE("trims content and comment if indicated", "[util][stripLineComment]")
{
    std::string input{" content $$ comment "};
    auto [contentFull0, commentFull0]
        = sciformats::jdx::util::stripLineComment(input, false, false);
    auto [contentFull1, commentTrimmed1]
        = sciformats::jdx::util::stripLineComment(input, false, true);
    auto [contentTrimmed2, commentFull2]
        = sciformats::jdx::util::stripLineComment(input, true, false);
    auto [contentTrimmed3, commentTrimmed3]
        = sciformats::jdx::util::stripLineComment(input, true, true);

    REQUIRE(" content " == contentFull0);
    REQUIRE(" comment " == commentFull0.value());
    REQUIRE(" content " == contentFull1);
    REQUIRE("comment" == commentTrimmed1.value());
    REQUIRE("content" == contentTrimmed2);
    REQUIRE(" comment " == commentFull2.value());
    REQUIRE("content" == contentTrimmed3);
    REQUIRE("comment" == commentTrimmed3.value());
}

TEST_CASE("recognizes Bruker specific section",
    "[util][isBrukerSpecificSectionStart]")
{
    std::string brukerSectionStart{"$$ Bruker specific parameters"};
    std::string brukerSectionStartF1{"$$ Bruker specific parameters for F1"};
    std::string brukerSectionEnd{"$$ End of Bruker specific parameters"};
    std::string brukerDashes{"$$ ---------------------------------"};
    std::string regularLdr{"##ORIGIN= Test"};

    REQUIRE(sciformats::jdx::util::isBrukerSpecificSectionStart(
        brukerSectionStart));
    REQUIRE(sciformats::jdx::util::isBrukerSpecificSectionStart(
        brukerSectionStartF1));
    REQUIRE_FALSE(
        sciformats::jdx::util::isBrukerSpecificSectionStart(brukerSectionEnd));
    REQUIRE_FALSE(
        sciformats::jdx::util::isBrukerSpecificSectionStart(brukerDashes));
    REQUIRE_FALSE(
        sciformats::jdx::util::isBrukerSpecificSectionStart(regularLdr));
}
