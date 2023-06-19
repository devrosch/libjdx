#include "jdx/api/JdxConverter.hpp"

#include "catch2/catch.hpp"

TEST_CASE("JdxConverter only maps valid JCAMP-DX", "[JdxConverter]")
{
    using namespace sciformats::api;
    using namespace sciformats::jdx::api;

    SECTION("Maps valid JCAMP-DX file")
    {
        auto converter = JdxConverter("resources/CompoundFile.jdx");
        auto rootNode = converter.read("/");

        REQUIRE("Root LINK BLOCK" == rootNode.name);
        REQUIRE(rootNode.parameters.size() == 4);
        REQUIRE(rootNode.data.empty());
        REQUIRE(rootNode.childNodeNames.size() == 4);

        SECTION("Maps nested XYDATA node")
        {
            auto nestedNode0 = converter.read("/0");
            REQUIRE("Data XYDATA (PAC) Block" == nestedNode0.name);
            REQUIRE(nestedNode0.parameters.size() == 11);
            REQUIRE(nestedNode0.childNodeNames.empty());
            auto data = nestedNode0.data;
            REQUIRE(data.size() == 2);
            REQUIRE(data.at(0).x == Approx(450.0));
            REQUIRE(data.at(0).y == Approx(10.0));
        }

        SECTION("Maps nested RADATA node")
        {
            auto nestedNode1 = converter.read("/1");
            REQUIRE("Data RADATA (PAC) Block" == nestedNode1.name);
            REQUIRE(nestedNode1.parameters.size() == 10);
            REQUIRE(nestedNode1.childNodeNames.empty());

            auto data = nestedNode1.data;
            REQUIRE(data.size() == 3);
            REQUIRE(data.at(0).x == Approx(0.0));
            REQUIRE(data.at(0).y == Approx(10.0));
        }

        SECTION("Maps nested XYPOINTS node")
        {
            auto nestedNode2 = converter.read("/2");
            REQUIRE("Data XYPOINTS (AFFN) Block" == nestedNode2.name);
            REQUIRE(nestedNode2.parameters.size() == 10);
            REQUIRE(nestedNode2.childNodeNames.empty());

            auto data = nestedNode2.data;
            REQUIRE(data.size() == 4);
            REQUIRE(data.at(0).x == Approx(900.0));
            REQUIRE(data.at(0).y == Approx(100.0));
        }

        SECTION("Throws when trying to read non existent node")
        {
            REQUIRE_THROWS(converter.read("/0/0"));
        }
    }

    SECTION("Throws when trying to map invalid JCAMP-DX file")
    {
        REQUIRE_THROWS(JdxConverter("resources/dummy.txt"));
    }

    SECTION("Throws when trying to map non-existing JCAMP-DX file")
    {
        REQUIRE_THROWS(JdxConverter("resources/non_existent.txt"));
    }
}
