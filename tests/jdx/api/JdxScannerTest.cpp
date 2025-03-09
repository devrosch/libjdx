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
