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
#include "util/StringUtils.hpp"

#include "catch2/catch.hpp"

#include <iostream>
#include <sstream>
#include <string>

TEST_CASE("left trims white space", "[util][trimLeft]")
{
    std::string actual{"\t\n\v\f\r abc \t\n\v\f\r"};
    std::string expect{"abc \t\n\v\f\r"};

    sciformats::jdx::util::trimLeft(actual);

    REQUIRE(expect == actual);
}

TEST_CASE("right trims white space", "[util][trimRight]")
{
    std::string actual{"\t\n\v\f\r abc \t\n\v\f\r"};
    std::string expect{"\t\n\v\f\r abc"};

    sciformats::jdx::util::trimRight(actual);

    REQUIRE(expect == actual);
}

TEST_CASE("trims white space", "[util][trim]")
{
    std::string actual{"\t\n\v\f\r abc \t\n\v\f\r"};
    std::string expect{"abc"};

    sciformats::jdx::util::trim(actual);

    REQUIRE(expect == actual);
}

TEST_CASE("white spaces recognized", "[util][isSpace]")
{
    std::string actual{" \t\n\v\f\raA\x80\xFF"};

    REQUIRE(sciformats::jdx::util::isSpace(actual.at(0)));
    REQUIRE(sciformats::jdx::util::isSpace(actual.at(1)));
    REQUIRE(sciformats::jdx::util::isSpace(actual.at(2)));
    REQUIRE(sciformats::jdx::util::isSpace(actual.at(3)));
    REQUIRE(sciformats::jdx::util::isSpace(actual.at(4)));
    REQUIRE(sciformats::jdx::util::isSpace(actual.at(5)));
    REQUIRE_FALSE(sciformats::jdx::util::isSpace(actual.at(6)));
    REQUIRE_FALSE(sciformats::jdx::util::isSpace(actual.at(7)));
    REQUIRE_FALSE(sciformats::jdx::util::isSpace(actual.at(8)));
    REQUIRE_FALSE(sciformats::jdx::util::isSpace(actual.at(9)));
}

TEST_CASE("lower case letters for ASCII produced", "[util][toLower]")
{
    std::string actual{"\t\n\v\f\raAzZ%?"};
    const std::string expected{"\t\n\v\f\raazz%?"};
    // does not work for non ASCII characters such as umlauts
    sciformats::jdx::util::toLower(actual);

    REQUIRE(expected == actual);
}

TEST_CASE("splits string without trimming", "[util][split]")
{
    const std::string fixture{"  this, is , a ,test   "};
    const std::vector<std::string> expected{"  this", " is ", " a ", "test   "};
    const auto output = sciformats::jdx::util::split(fixture, ",");

    REQUIRE(expected == output);
}

TEST_CASE("splits string with trimming", "[util][split]")
{
    const std::string fixture{"  this, is , a ,test   "};
    const std::vector<std::string> expected{"this", "is", "a", "test"};
    const auto output = sciformats::jdx::util::split(fixture, ",", true);

    REQUIRE(expected == output);
}

TEST_CASE("split retains trailing empty segment", "[util][split]")
{
    const std::string fixture{", is, a,"};
    const std::vector<std::string> expected{"", "is", "a", ""};
    const auto output = sciformats::jdx::util::split(fixture, ",", true);

    REQUIRE(expected == output);
}

TEST_CASE("split returns full string if no delimiter present", "[util][split]")
{
    const std::string fixture{" test "};
    const std::vector<std::string> expected{" test "};
    const auto output = sciformats::jdx::util::split(fixture, ",", false);

    REQUIRE(expected == output);
}
