#include "jdx/api/JdxDataMapper.hpp"
#include "api/Node2.hpp"

#include "catch2/catch.hpp"

TEST_CASE("JdxDataMapper only maps valid JCAMP-DX", "[JdxDataMapper]")
{
    using namespace sciformats::api;
    using namespace sciformats::jdx::api;

    SECTION("Maps valid JCAMP-DX file")
    {
        auto mapper = JdxDataMapper("resources/CompoundFile.jdx");
        auto rootNode = mapper.read("/");

        REQUIRE("Root LINK BLOCK" == rootNode.name);
        REQUIRE(rootNode.parameters.size() == 4);
        REQUIRE(rootNode.data.empty());
        REQUIRE(rootNode.childNodeNames.size() == 4);

        SECTION("Maps nested XYDATA node")
        {
            auto nestedNode0 = mapper.read("/0");
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
            auto nestedNode1 = mapper.read("/1");
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
            auto nestedNode2 = mapper.read("/2");
            REQUIRE("Data XYPOINTS (AFFN) Block" == nestedNode2.name);
            REQUIRE(nestedNode2.parameters.size() == 10);
            REQUIRE(nestedNode2.childNodeNames.empty());

            auto data = nestedNode2.data;
            REQUIRE(data.size() == 4);
            REQUIRE(data.at(0).x == Approx(900.0));
            REQUIRE(data.at(0).y == Approx(100.0));
        }
    }

    SECTION("Throws when trying to map invalid or non-existing JCAMP-DX file")
    {
        REQUIRE_THROWS(JdxDataMapper("resources/dummy.txt"));
        REQUIRE_THROWS(JdxDataMapper("resources/non_existent.txt"));
    }
}
