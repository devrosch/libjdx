#include "jdx/api/JdxConverter.hpp"

#include "catch2/catch.hpp"

#include <iostream>

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
        REQUIRE(rootNode.childNodeNames.size() == 6);

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

        SECTION("Maps nested NTUPLES node")
        {
            auto nestedNode4 = converter.read("/3");
            REQUIRE("NTUPLES Block" == nestedNode4.name);

            REQUIRE(nestedNode4.parameters.size() == 3);
            REQUIRE(nestedNode4.childNodeNames.size() == 1);
            REQUIRE(nestedNode4.childNodeNames.at(0) == "NMR SPECTRUM");

            auto nTuplesNode = converter.read("/3/0");

            REQUIRE(nTuplesNode.name == "NMR SPECTRUM");
            REQUIRE(nTuplesNode.data.empty());
            REQUIRE(nTuplesNode.childNodeNames.size() == 2);
            REQUIRE(nTuplesNode.parameters.size() == 13);

            REQUIRE(nTuplesNode.parameters.at(0).key == "VAR_NAME");
            REQUIRE(nTuplesNode.parameters.at(0).value
                    == "FREQUENCY, SPECTRUM/REAL, SPECTRUM/IMAG, PAGE NUMBER");
            REQUIRE(nTuplesNode.parameters.at(11).key == "$CUSTOMLDR");
            REQUIRE(nTuplesNode.parameters.at(11).value
                    == "VAL1, VAL2, VAL3, VAL4");
            REQUIRE(nTuplesNode.parameters.at(12).key == "$CUSTOMLDR2");
            REQUIRE(nTuplesNode.parameters.at(12).value
                    == ", , VAL3, VAL4");

            auto page1 = converter.read("/3/0/0");
            REQUIRE(page1.name == "N=1");
            REQUIRE(page1.parameters.size() == 1);
            REQUIRE(page1.parameters.at(0).key == "Plot Descriptor");
            REQUIRE(page1.parameters.at(0).value == "XYDATA");

            auto page1Data = page1.data;
            REQUIRE(page1Data.size() == 4);
            REQUIRE(page1Data.at(0).x == Approx(0.1));
            REQUIRE(page1Data.at(0).y == Approx(50.0));
            REQUIRE(page1Data.at(3).x == Approx(0.25));
            REQUIRE(page1Data.at(3).y == Approx(105.0));

            auto page2 = converter.read("/3/0/1");
            REQUIRE(page2.name == "N=2");
            REQUIRE(page2.parameters.size() == 1);
            REQUIRE(page2.parameters.at(0).key == "Plot Descriptor");
            REQUIRE(page2.parameters.at(0).value == "XYDATA");

            auto page2Data = page2.data;
            REQUIRE(page2Data.size() == 4);
            REQUIRE(page2Data.at(0).x == Approx(0.1));
            REQUIRE(page2Data.at(0).y == Approx(300.0));
            REQUIRE(page2Data.at(3).x == Approx(0.25));
            REQUIRE(page2Data.at(3).y == Approx(410.0));
        }

        // TODO: test /4 (PEAK TABLE)
        // TODO: add data/test for PEAK ASSIGNMENTS

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
