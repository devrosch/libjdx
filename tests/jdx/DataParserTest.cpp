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
#include "util/DataParser.hpp"

#include "catch2/catch.hpp"

#include <istream>
#include <sstream>

TEST_CASE("parses AFFN data line", "[DataParser]")
{
    std::string input{"1.23 4.5E23 4.5e2 7.89E-14 600 1E2"};

    auto [actual, difEncoded]
        = libjdx::jdx::util::DataParser::readValues(input, false);
    auto expect = std::vector<double>{1.23, 4.5E23, 4.5E2, 7.89E-14, 600, 1E2};

    REQUIRE(false == difEncoded);
    REQUIRE(6 == actual.size());
    for (size_t i{0}; i < expect.size(); i++)
    {
        REQUIRE((expect.at(i) == Approx(actual.at(i))));
    }
}

TEST_CASE("parses ambiguous AFFN - SQZ data line", "[DataParser]")
{
    std::string input{"1E2 B23C34D45E56"};

    auto [actual, difEncoded]
        = libjdx::jdx::util::DataParser::readValues(input, true);
    auto expect = std::vector<double>{100, 223, 334, 445, 556};

    REQUIRE(false == difEncoded);
    REQUIRE(expect.size() == actual.size());
    for (size_t i{0}; i < expect.size(); i++)
    {
        REQUIRE((expect.at(i) == Approx(actual.at(i))));
    }
}

TEST_CASE("parses FIX (I3) ASCII data line", "[DataParser]")
{
    std::string input{"1  2  3  3  2  1  0 -1 -2 -3"};

    auto [actual, difEncoded]
        = libjdx::jdx::util::DataParser::readValues(input, false);
    auto expect = std::vector<double>{1, 2, 3, 3, 2, 1, 0, -1, -2, -3};

    REQUIRE(false == difEncoded);
    REQUIRE(expect.size() == actual.size());
    for (size_t i{0}; i < expect.size(); i++)
    {
        REQUIRE((expect.at(i) == Approx(actual.at(i))));
    }
}

TEST_CASE("parses PAC data line", "[DataParser]")
{
    std::string input{"1+2+3+3+2+1+0-1-2-3"};

    auto [actual, difEncoded]
        = libjdx::jdx::util::DataParser::readValues(input, true);
    auto expect = std::vector<double>{1, 2, 3, 3, 2, 1, 0, -1, -2, -3};

    REQUIRE(false == difEncoded);
    REQUIRE(expect.size() == actual.size());
    for (size_t i{0}; i < expect.size(); i++)
    {
        REQUIRE((expect.at(i) == Approx(actual.at(i))));
    }
}

TEST_CASE("parses SQZ data line", "[DataParser]")
{
    std::string input{"1BCCBA@abc"};

    auto [actual, difEncoded]
        = libjdx::jdx::util::DataParser::readValues(input, true);
    auto expect = std::vector<double>{1, 2, 3, 3, 2, 1, 0, -1, -2, -3};

    REQUIRE(false == difEncoded);
    REQUIRE(expect.size() == actual.size());
    for (size_t i{0}; i < expect.size(); i++)
    {
        REQUIRE((expect.at(i) == Approx(actual.at(i))));
    }
}

TEST_CASE("parses DIF data line", "[DataParser]")
{
    std::string input{"1JJ%jjjjjj"};

    auto [actual, difEncoded]
        = libjdx::jdx::util::DataParser::readValues(input, true);
    auto expect = std::vector<double>{1, 2, 3, 3, 2, 1, 0, -1, -2, -3};

    REQUIRE(true == difEncoded);
    REQUIRE(expect.size() == actual.size());
    for (size_t i{0}; i < expect.size(); i++)
    {
        REQUIRE((expect.at(i) == Approx(actual.at(i))));
    }
}

TEST_CASE("fails if sequence starts with DIF token", "[DataParser]")
{
    std::string input{"jjj"};

    REQUIRE_THROWS(libjdx::jdx::util::DataParser::readValues(input, true));
}

TEST_CASE("parses DIFDUP data line", "[DataParser]")
{
    std::string input{"1JT%jX"};

    auto [actual, difEncoded]
        = libjdx::jdx::util::DataParser::readValues(input, true);
    auto expect = std::vector<double>{1, 2, 3, 3, 2, 1, 0, -1, -2, -3};

    // last ordinate is in DUP format, but previous value is DIF hence count as
    // DIF (as Bruker does)
    REQUIRE(true == difEncoded);
    REQUIRE(expect.size() == actual.size());
    for (size_t i{0}; i < expect.size(); i++)
    {
        REQUIRE((expect.at(i) == Approx(actual.at(i))));
    }
}

TEST_CASE(
    "fails if sequence contains two consecutive DUP tokens", "[DataParser]")
{
    std::string input{"1VZ"};

    REQUIRE_THROWS(libjdx::jdx::util::DataParser::readValues(input, true));
}

TEST_CASE("fails for illegal token start character", "[DataParser]")
{
    // "u" is an illegal character
    std::string input{"123 u45"};

    REQUIRE_THROWS(libjdx::jdx::util::DataParser::readValues(input, true));
}

TEST_CASE("parses mixed PAC/AFFN stream", "[DataParser]")
{
    std::string input{
        "599.860 0 0 0 0 2 4 4 4 7 5 4 4 5 5 7 10 11 11 6 5 7 6 9 9 7\r\n"
        "648.081 10 10 9 10 11 12 15 16 16 14 17 38 38 35 38 42 47 54\r\n"
        "682.799  59  66  75  78  88  96 104 110 121 128\r\n"
        "##END="};
    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    auto actual = libjdx::jdx::util::DataParser::readXppYYData(reader);
    auto expect = std::vector<double>{0, 0, 0, 0, 2, 4, 4, 4, 7, 5, 4, 4, 5, 5,
        7, 10, 11, 11, 6, 5, 7, 6, 9, 9, 7, 10, 10, 9, 10, 11, 12, 15, 16, 16,
        14, 17, 38, 38, 35, 38, 42, 47, 54, 59, 66, 75, 78, 88, 96, 104, 110,
        121, 128};
    std::string lastLine = reader.readLine();

    REQUIRE(expect.size() == actual.size());
    REQUIRE(std::string{"##END="} == lastLine);
    for (size_t i{0}; i < expect.size(); i++)
    {
        REQUIRE((expect.at(i) == Approx(actual.at(i))));
    }
}

TEST_CASE("detects failing Y check", "[DataParser]")
{
    // first line ends with y value 3, next line should duplicate it but is 4
    std::string input{"599.000+1jj\r\n"
                      "600.000+4jj\r\n"
                      "##END="};
    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    REQUIRE_THROWS(libjdx::jdx::util::DataParser::readXppYYData(reader));
}

TEST_CASE("parses DIFDUP stream", "[DataParser]")
{
    std::string input{
        "599.860@VKT%TLkj%J%KLJ%njKjL%kL%jJULJ%kLK1%lLMNPNPRLJ0QTOJ1P\r\n"
        "700.158A28\r\n"
        "##END="};
    auto streamPtr = std::make_unique<std::stringstream>(std::ios_base::in);
    streamPtr->str(input);
    libjdx::io::TextReader reader{std::move(streamPtr)};

    auto actual = libjdx::jdx::util::DataParser::readXppYYData(reader);
    auto expect = std::vector<double>{0, 0, 0, 0, 2, 4, 4, 4, 7, 5, 4, 4, 5, 5,
        7, 10, 11, 11, 6, 5, 7, 6, 9, 9, 7, 10, 10, 9, 10, 11, 12, 15, 16, 16,
        14, 17, 38, 38, 35, 38, 42, 47, 54, 59, 66, 75, 78, 88, 96, 104, 110,
        121, 128};
    std::string lastLine = reader.readLine();

    REQUIRE(expect.size() == actual.size());
    REQUIRE(std::string{"##END="} == lastLine);
    for (size_t i{0}; i < expect.size(); i++)
    {
        REQUIRE((expect.at(i) == Approx(actual.at(i))));
    }
}
