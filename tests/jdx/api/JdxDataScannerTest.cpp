#include "jdx/api/JdxDataScanner.hpp"
#include "api/Node2.hpp"

#include "catch2/catch.hpp"

TEST_CASE(
    "JdxDataScanner only accepts to valid JCAMP-DX", "[JdxDataScanner]")
{
    using namespace sciformats::api;
    using namespace sciformats::jdx::api;
    JdxDataScanner scanner{};

    SECTION("Only recognizes existing file that contain JCAMP-DX data")
    {
        REQUIRE_FALSE(scanner.isRecognized("resources/dummy.txt"));
        REQUIRE_FALSE(scanner.isRecognized("resources/non_existent.txt"));
        REQUIRE(scanner.isRecognized("resources/Claniline.jdx"));
        REQUIRE(scanner.isRecognized("resources/CompoundFile.jdx"));
    }
}
