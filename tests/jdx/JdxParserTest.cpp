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
#define CATCH_CONFIG_MAIN
#include "jdx/JdxParser.hpp"

#include "catch2/catch.hpp"

#include <array>
#include <climits>
#include <string>

TEST_CASE("accepts legal file", "[JdxParser]")
{
    const std::string path{"resources/SimpleFile.jdx"};
    std::ifstream istream{path};

    bool canParse = libjdx::jdx::JdxParser::canParse(path, istream);
    REQUIRE(canParse == true);
}

TEST_CASE("accepts legal header with whitespaces", "[JdxParser]")
{
    const std::string path{"legal.jdx"};
    const std::string header{"  ##TI_TLE= xxx\n"};
    std::stringstream istream{header};

    bool canParse = libjdx::jdx::JdxParser::canParse(path, istream);
    REQUIRE(canParse == true);
}

TEST_CASE("rejects illegal file (wrong extension)", "[JdxParser]")
{
    const std::string path{"resources/dummy.txt"};
    std::ifstream istream{path};

    bool canParse = libjdx::jdx::JdxParser::canParse(path, istream);
    REQUIRE(canParse == false);
}

TEST_CASE("rejects illegal file (wrong magic bytes)", "[JdxParser]")
{
    const std::string path{"resources/dummy.jdx"};
    std::ifstream istream{path};

    bool canParse = libjdx::jdx::JdxParser::canParse(path, istream);
    REQUIRE(canParse == false);
}

TEST_CASE("parse succeeds for legal file", "[JdxParser]")
{
    const std::string path{"resources/SimpleFile.jdx"};
    auto istream = std::make_unique<std::ifstream>(path);

    REQUIRE_NOTHROW(libjdx::jdx::JdxParser::parse(std::move(istream)));
}

TEST_CASE("parse throws for illegal file", "[JdxParser]")
{
    const std::string path{"resources/dummy.jdx"};
    auto istream = std::make_unique<std::ifstream>(path);

    REQUIRE_THROWS(libjdx::jdx::JdxParser::parse(std::move(istream)));
}
