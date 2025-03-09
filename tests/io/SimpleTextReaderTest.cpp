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
#include "io/SimpleTextReader.hpp"

#include "catch2/catch.hpp"

#include <memory>
#include <sstream>
#include <string>

TEST_CASE("SimpleTextReader reads data", "[SimpleTextReader]")
{
    SECTION("reads file specified by path", "[SimpleTextReader]")
    {
        const std::string path{"resources/dummy.txt"};
        libjdx::io::SimpleTextReader reader{path};

        REQUIRE(0 == reader.tellg());
        REQUIRE(20 == reader.getLength());
        REQUIRE(0 == reader.tellg());
        REQUIRE_FALSE(reader.eof());
        REQUIRE("not a JCAMP-DX file" == reader.readLine());
        REQUIRE(reader.eof());
        REQUIRE(20 == reader.tellg());
        reader.seekg(1);
        REQUIRE(1 == reader.tellg());
    }

    SECTION("reads data provided by an istream", "[SimpleTextReader]")
    {
        std::string input{"line 1\r\n"
                          "line 2\n"
                          "line 3"};
        auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
        streamPtr->str(input);
        libjdx::io::SimpleTextReader reader{std::move(streamPtr)};

        REQUIRE(0 == reader.tellg());
        REQUIRE(21 == reader.getLength());
        REQUIRE(0 == reader.tellg());
        REQUIRE_FALSE(reader.eof());
        REQUIRE("line 1" == reader.readLine());
        REQUIRE_FALSE(reader.eof());
        REQUIRE(8 == reader.tellg());
        REQUIRE("line 2" == reader.readLine());
        REQUIRE_FALSE(reader.eof());
        REQUIRE(15 == reader.tellg());
        REQUIRE("line 3" == reader.readLine());
        REQUIRE(reader.eof());
        REQUIRE(21 == reader.tellg());
        reader.seekg(1);
        REQUIRE(1 == reader.tellg());
        reader.seekg(21);
        REQUIRE(21 == reader.tellg());
        reader.seekg(8);
        REQUIRE("line 2" == reader.readLine());
    }

    SECTION("throws when trying to read past end", "[SimpleTextReader]")
    {
        std::string input{};
        auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
        streamPtr->str(input);
        libjdx::io::SimpleTextReader reader{std::move(streamPtr)};

        REQUIRE_THROWS(reader.readLine());
    }
}
