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
#include "jdx/api/JdxScanner.hpp"

#include "catch2/catch.hpp"

TEST_CASE("JdxScanner recognizes valid JCAMP-DX and provides a converter for "
          "the data",
    "[JdxScanner]")
{
    using namespace sciformats::api;
    using namespace sciformats::jdx::api;
    JdxScanner scanner{};

    SECTION("Recognizes JCAMP-DX data")
    {
        const char* simpleFilePath = "resources/SimpleFile.jdx";
        const char* compoundFilePath = "resources/CompoundFile.jdx";

        REQUIRE(scanner.isRecognized(simpleFilePath));
        REQUIRE(scanner.isRecognized(compoundFilePath));

        SECTION("Provides converter for JCAMP-DX data")
        {
            REQUIRE_NOTHROW(scanner.getConverter(simpleFilePath));
            REQUIRE_NOTHROW(scanner.getConverter(compoundFilePath));
        }
    }

    SECTION("Rejects non JCAMP-DX data")
    {
        const char* dummyPath = "resources/dummy.txt";
        const char* nonExistentPath = "resources/non_existent.txt";

        REQUIRE_FALSE(scanner.isRecognized(dummyPath));
        REQUIRE_FALSE(scanner.isRecognized(nonExistentPath));

        SECTION("Throws when converter for non JCAMP-DX data is requested")
        {
            REQUIRE_THROWS(scanner.getConverter(dummyPath));
        }

        SECTION("Throws when converter for non existent data is requested")
        {
            REQUIRE_THROWS(scanner.getConverter(nonExistentPath));
        }
    }
}
