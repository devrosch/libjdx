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
        const char* clanilinePath = "resources/Claniline.jdx";
        const char* compoundFilePath = "resources/CompoundFile.jdx";

        REQUIRE(scanner.isRecognized(clanilinePath));
        REQUIRE(scanner.isRecognized(compoundFilePath));

        SECTION("Provides converter for JCAMP-DX data")
        {
            REQUIRE_NOTHROW(scanner.getConverter(clanilinePath));
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
