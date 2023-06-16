#include "jdx/api/JdxScanner.hpp"

#include "catch2/catch.hpp"

TEST_CASE("JdxScanner only accepts to valid JCAMP-DX", "[JdxScanner]")
{
    using namespace sciformats::api;
    using namespace sciformats::jdx::api;
    JdxScanner scanner{};

    SECTION("Only recognizes existing file that contain JCAMP-DX data")
    {
        REQUIRE_FALSE(scanner.isRecognized("resources/dummy.txt"));
        REQUIRE_FALSE(scanner.isRecognized("resources/non_existent.txt"));
        REQUIRE(scanner.isRecognized("resources/Claniline.jdx"));
        REQUIRE(scanner.isRecognized("resources/CompoundFile.jdx"));
    }
}
