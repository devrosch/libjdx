#include "jdx/BufferedTextReader.hpp"

#include "catch2/catch.hpp"

#include <memory>
#include <sstream>
#include <string>

TEST_CASE("BufferedTextReader reads data", "[BufferedTextReader]")
{
    SECTION("reads file specified by path", "[BufferedTextReader]")
    {
        const std::string path{"resources/dummy.txt"};
        sciformats::jdx::BufferedTextReader reader{path, 5};

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

    SECTION("reads data provided by an istream", "[BufferedTextReader]")
    {
        std::string input{"line 1\r\n"
                          "line 2\n"
                          "line 3"};
        auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
        streamPtr->str(input);
        sciformats::jdx::BufferedTextReader reader{std::move(streamPtr), 5};

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

    SECTION("throws when trying to read past end", "[BufferedTextReader]")
    {
        std::string input{};
        auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
        streamPtr->str(input);
        sciformats::jdx::BufferedTextReader reader{std::move(streamPtr), 5};

        REQUIRE_THROWS(reader.readLine());
    }
}
