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
#include "jdx/api/JdxConverter.hpp"

#include "catch2/catch.hpp"

#include <iostream>

// NOLINTNEXTLINE(hicpp-function-size,readability-function-size)
TEST_CASE("JdxConverter only maps valid JCAMP-DX", "[JdxConverter]")
{
    using namespace libjdx::api;
    using namespace libjdx::jdx::api;

    SECTION("Maps valid JCAMP-DX file")
    {
        auto converter = JdxConverter("resources/CompoundFile.jdx");
        auto rootNode = converter.read("/");

        REQUIRE("Root LINK BLOCK" == rootNode.name);
        REQUIRE(rootNode.parameters.size() == 4);
        REQUIRE(rootNode.data.empty());
        REQUIRE(rootNode.childNodeNames.size() == 9);

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
            auto nestedNode3 = converter.read("/3");
            REQUIRE("NTUPLES Block" == nestedNode3.name);

            REQUIRE(nestedNode3.parameters.size() == 3);
            REQUIRE(nestedNode3.childNodeNames.size() == 1);
            REQUIRE(nestedNode3.childNodeNames.at(0) == "NMR SPECTRUM");

            auto nTuplesNode = converter.read("/3/0");

            REQUIRE(nTuplesNode.name == "NMR SPECTRUM");
            REQUIRE(nTuplesNode.data.empty());
            REQUIRE(nTuplesNode.childNodeNames.size() == 2);
            REQUIRE(nTuplesNode.parameters.size() == 13);

            REQUIRE(nTuplesNode.parameters.at(0).key == "VARNAME");
            REQUIRE(nTuplesNode.parameters.at(0).value
                    == "FREQUENCY,    SPECTRUM/REAL,    SPECTRUM/IMAG, PAGE "
                       "NUMBER");
            REQUIRE(nTuplesNode.parameters.at(11).key == "$CUSTOMLDR");
            REQUIRE(
                nTuplesNode.parameters.at(11).value
                == "VAL1,             VAL2,             VAL3,        VAL4,");
            REQUIRE(nTuplesNode.parameters.at(12).key == "$CUSTOMLDR2");
            REQUIRE(nTuplesNode.parameters.at(12).value
                    == ",                 ,             VAL3,        VAL4");

            auto page1 = converter.read("/3/0/0");
            REQUIRE(page1.name == "N=1 - SPECTRUM/REAL");
            REQUIRE(page1.parameters.size() == 1);
            REQUIRE(page1.parameters.at(0).key == "Plot Descriptor");
            REQUIRE(page1.parameters.at(0).value == "XYDATA");

            auto page1Meta = page1.metadata;
            REQUIRE(page1Meta.size() == 4);
            REQUIRE(page1Meta.at("x.label") == "X");
            REQUIRE(page1Meta.at("x.unit") == "HZ");
            REQUIRE(page1Meta.at("y.label") == "R");
            REQUIRE(page1Meta.at("y.unit") == "ARBITRARY UNITS");

            auto page1Data = page1.data;
            REQUIRE(page1Data.size() == 4);
            REQUIRE(page1Data.at(0).x == Approx(0.1));
            REQUIRE(page1Data.at(0).y == Approx(50.0));
            REQUIRE(page1Data.at(3).x == Approx(0.25));
            REQUIRE(page1Data.at(3).y == Approx(105.0));

            auto page2 = converter.read("/3/0/1");
            REQUIRE(page2.name == "N=2 - SPECTRUM/IMAG");
            REQUIRE(page2.parameters.size() == 1);
            REQUIRE(page2.parameters.at(0).key == "Plot Descriptor");
            REQUIRE(page2.parameters.at(0).value == "XYDATA");

            auto page2Meta = page2.metadata;
            REQUIRE(page2Meta.size() == 4);
            REQUIRE(page2Meta.at("x.label") == "X");
            REQUIRE(page2Meta.at("x.unit") == "HZ");
            REQUIRE(page2Meta.at("y.label") == "I");
            REQUIRE(page2Meta.at("y.unit") == "ARBITRARY UNITS");

            auto page2Data = page2.data;
            REQUIRE(page2Data.size() == 4);
            REQUIRE(page2Data.at(0).x == Approx(0.1));
            REQUIRE(page2Data.at(0).y == Approx(300.0));
            REQUIRE(page2Data.at(3).x == Approx(0.25));
            REQUIRE(page2Data.at(3).y == Approx(410.0));
        }

        SECTION("Maps nested PEAK TABLE node")
        {
            auto nestedNode4 = converter.read("/4");
            REQUIRE("PEAK TABLE (AFFN) Block" == nestedNode4.name);
            REQUIRE(nestedNode4.parameters.size() == 10);
            REQUIRE(nestedNode4.data.empty());
            REQUIRE(nestedNode4.childNodeNames.empty());

            auto peakTable = nestedNode4.table;
            REQUIRE(peakTable.columnNames.size() == 2);
            REQUIRE(peakTable.columnNames.at(0).first == "x");
            REQUIRE(peakTable.columnNames.at(0).second == "Peak Position");
            REQUIRE(peakTable.columnNames.at(1).first == "y");
            REQUIRE(peakTable.columnNames.at(1).second == "Intensity");

            REQUIRE(peakTable.rows.at(0).at("x") == "450.000000");
            REQUIRE(peakTable.rows.at(0).at("y") == "10.000000");
            REQUIRE(peakTable.rows.at(1).at("x") == "451.000000");
            REQUIRE(peakTable.rows.at(1).at("y") == "11.000000");
            REQUIRE(peakTable.rows.at(2).at("x") == "460.000000");
            REQUIRE(peakTable.rows.at(2).at("y") == "20.000000");
            REQUIRE(peakTable.rows.at(3).at("x") == "461.000000");
            REQUIRE(peakTable.rows.at(3).at("y") == "21.000000");
        }

        SECTION("Maps nested PEAK ASSIGNMENTS node")
        {
            auto nestedNode5 = converter.read("/5");
            REQUIRE("PEAK ASSIGNMENTS (AFFN) Block" == nestedNode5.name);
            REQUIRE(nestedNode5.parameters.size() == 10);
            REQUIRE(nestedNode5.data.empty());
            REQUIRE(nestedNode5.childNodeNames.empty());

            auto peakTable = nestedNode5.table;
            REQUIRE(peakTable.columnNames.size() == 4);
            REQUIRE(peakTable.columnNames.at(0).first == "x");
            REQUIRE(peakTable.columnNames.at(0).second == "Peak Position");
            REQUIRE(peakTable.columnNames.at(1).first == "y");
            REQUIRE(peakTable.columnNames.at(1).second == "Intensity");
            REQUIRE(peakTable.columnNames.at(2).first == "m");
            REQUIRE(peakTable.columnNames.at(2).second == "Multiplicity");
            REQUIRE(peakTable.columnNames.at(3).first == "a");
            REQUIRE(peakTable.columnNames.at(3).second == "Assignment");

            REQUIRE(peakTable.rows.at(0).at("x") == "450.000000");
            REQUIRE(peakTable.rows.at(0).at("y") == "10.000000");
            REQUIRE(peakTable.rows.at(0).at("m") == "S");
            REQUIRE(peakTable.rows.at(0).at("a") == "1");
            REQUIRE(peakTable.rows.at(1).at("x") == "451.000000");
            REQUIRE(peakTable.rows.at(1).at("y") == "11.000000");
            REQUIRE(peakTable.rows.at(1).at("m") == "T");
            REQUIRE(peakTable.rows.at(1).at("a") == "2");
            REQUIRE(peakTable.rows.at(2).at("x") == "460.000000");
            REQUIRE(peakTable.rows.at(2).at("y") == "20.000000");
            REQUIRE(peakTable.rows.at(2).at("m") == "D");
            REQUIRE(peakTable.rows.at(2).at("a") == "3");
            REQUIRE(peakTable.rows.at(3).at("x") == "461.000000");
            REQUIRE(peakTable.rows.at(3).at("y") == "21.000000");
            REQUIRE(peakTable.rows.at(3).at("m") == "Q");
            REQUIRE(peakTable.rows.at(3).at("a") == "4");
        }

        SECTION("Maps mass spectrum PEAK TABLE node as data and peaks")
        {
            auto nestedNode6 = converter.read("/6");
            REQUIRE("MS PEAK TABLE Block" == nestedNode6.name);
            REQUIRE(nestedNode6.parameters.size() == 7);
            REQUIRE(nestedNode6.data.size() == 4);
            REQUIRE(nestedNode6.metadata.size() == 3);
            REQUIRE(nestedNode6.childNodeNames.empty());

            auto data = nestedNode6.data;
            REQUIRE(data.size() == 4);
            REQUIRE(data.at(0).x == Approx(50.0));
            REQUIRE(data.at(0).y == Approx(10.0));
            REQUIRE(data.at(1).x == Approx(51.0));
            REQUIRE(data.at(1).y == Approx(11.0));
            REQUIRE(data.at(2).x == Approx(130.0));
            REQUIRE(data.at(2).y == Approx(20.0));
            REQUIRE(data.at(3).x == Approx(131.0));
            REQUIRE(data.at(3).y == Approx(21.0));

            auto peakTable = nestedNode6.table;
            REQUIRE(peakTable.columnNames.size() == 2);
            REQUIRE(peakTable.columnNames.at(0).first == "x");
            REQUIRE(peakTable.columnNames.at(0).second == "Peak Position");
            REQUIRE(peakTable.columnNames.at(1).first == "y");
            REQUIRE(peakTable.columnNames.at(1).second == "Intensity");

            REQUIRE(peakTable.rows.at(0).at("x") == "50.000000");
            REQUIRE(peakTable.rows.at(0).at("y") == "10.000000");
            REQUIRE(peakTable.rows.at(1).at("x") == "51.000000");
            REQUIRE(peakTable.rows.at(1).at("y") == "11.000000");
            REQUIRE(peakTable.rows.at(2).at("x") == "130.000000");
            REQUIRE(peakTable.rows.at(2).at("y") == "20.000000");
            REQUIRE(peakTable.rows.at(3).at("x") == "131.000000");
            REQUIRE(peakTable.rows.at(3).at("y") == "21.000000");

            auto metadata = nestedNode6.metadata;
            REQUIRE(metadata.count("x.unit") == 1);
            REQUIRE(metadata.at("x.unit") == "M/Z");
            REQUIRE(metadata.count("y.unit") == 1);
            REQUIRE(metadata.at("y.unit") == "RELATIVE ABUNDANCE");
            REQUIRE(metadata.count("plot.style") == 1);
            REQUIRE(metadata.at("plot.style") == "sticks");
        }

        SECTION(
            "Maps mass spectrum NTUPLES PEAK TABLE PAGE node as data and peaks")
        {
            auto nestedNode7 = converter.read("/7/0/0");
            REQUIRE("T=10 - INTENSITY" == nestedNode7.name);
            REQUIRE(nestedNode7.parameters.size() == 2);
            REQUIRE(nestedNode7.data.size() == 4);
            REQUIRE(nestedNode7.metadata.size() == 3);
            REQUIRE(nestedNode7.childNodeNames.empty());

            auto data = nestedNode7.data;
            REQUIRE(data.size() == 4);
            REQUIRE(data.at(0).x == Approx(50.0));
            REQUIRE(data.at(0).y == Approx(10.0));
            REQUIRE(data.at(1).x == Approx(51.0));
            REQUIRE(data.at(1).y == Approx(11.0));
            REQUIRE(data.at(2).x == Approx(130.0));
            REQUIRE(data.at(2).y == Approx(20.0));
            REQUIRE(data.at(3).x == Approx(131.0));
            REQUIRE(data.at(3).y == Approx(21.0));

            auto peakTable = nestedNode7.table;
            REQUIRE(peakTable.columnNames.size() == 2);
            REQUIRE(peakTable.columnNames.at(0).first == "x");
            REQUIRE(peakTable.columnNames.at(0).second == "Peak Position");
            REQUIRE(peakTable.columnNames.at(1).first == "y");
            REQUIRE(peakTable.columnNames.at(1).second == "Intensity");

            REQUIRE(peakTable.rows.at(0).at("x") == "50.000000");
            REQUIRE(peakTable.rows.at(0).at("y") == "10.000000");
            REQUIRE(peakTable.rows.at(1).at("x") == "51.000000");
            REQUIRE(peakTable.rows.at(1).at("y") == "11.000000");
            REQUIRE(peakTable.rows.at(2).at("x") == "130.000000");
            REQUIRE(peakTable.rows.at(2).at("y") == "20.000000");
            REQUIRE(peakTable.rows.at(3).at("x") == "131.000000");
            REQUIRE(peakTable.rows.at(3).at("y") == "21.000000");

            auto metadata = nestedNode7.metadata;
            REQUIRE(metadata.count("x.unit") == 1);
            REQUIRE(metadata.at("x.unit") == "M/Z");
            REQUIRE(metadata.count("y.unit") == 1);
            REQUIRE(metadata.at("y.unit") == "RELATIVE ABUNDANCE");
            REQUIRE(metadata.count("plot.style") == 1);
            REQUIRE(metadata.at("plot.style") == "sticks");
        }

        SECTION("Maps AUDIT TRAIL as node")
        {
            auto nestedNode8 = converter.read("/8/0");
            REQUIRE("AUDITTRAIL" == nestedNode8.name);
            REQUIRE(nestedNode8.parameters.empty());
            REQUIRE(nestedNode8.data.empty());
            REQUIRE(nestedNode8.metadata.empty());
            REQUIRE(nestedNode8.childNodeNames.empty());

            auto table = nestedNode8.table;
            REQUIRE(table.columnNames.size() == 7);
            REQUIRE(table.columnNames.at(0).first == "number");
            REQUIRE(table.columnNames.at(0).second == "NUMBER");
            REQUIRE(table.columnNames.at(1).first == "when");
            REQUIRE(table.columnNames.at(1).second == "WHEN");
            REQUIRE(table.columnNames.at(2).first == "who");
            REQUIRE(table.columnNames.at(2).second == "WHO");
            REQUIRE(table.columnNames.at(3).first == "where");
            REQUIRE(table.columnNames.at(3).second == "WHERE");
            REQUIRE(table.columnNames.at(4).first == "process");
            REQUIRE(table.columnNames.at(4).second == "PROCESS");
            REQUIRE(table.columnNames.at(5).first == "version");
            REQUIRE(table.columnNames.at(5).second == "VERSION");
            REQUIRE(table.columnNames.at(6).first == "what");
            REQUIRE(table.columnNames.at(6).second == "WHAT");

            REQUIRE(table.rows.size() == 3);
            auto entry0 = table.rows.at(0);
            REQUIRE(entry0.at("number") == "1");
            REQUIRE(entry0.at("when") == "2023-08-06 08:00:00.000 +0200");
            REQUIRE(entry0.at("who") == "testuser");
            REQUIRE(entry0.at("where") == "loc01");
            REQUIRE(entry0.at("process") == "proc1");
            REQUIRE(entry0.at("version") == "SW 1.3");
            REQUIRE(entry0.at("what") == "acquisition\nline 2\nline 3");
            auto entry1 = table.rows.at(1);
            REQUIRE(entry1.at("number") == "2");
            REQUIRE(entry1.at("when") == "2023-08-06 08:10:00.000 +0200");
            REQUIRE(entry1.at("who") == "testuser");
            REQUIRE(entry1.at("where") == "loc01");
            REQUIRE(entry1.at("process") == "proc1");
            REQUIRE(entry1.at("version") == "SW 1.3");
            REQUIRE(entry1.at("what") == "raw data processing\nline 2\nline 3");
            auto entry2 = table.rows.at(2);
            REQUIRE(entry2.at("number") == "3");
            REQUIRE(entry2.at("when") == "2023-08-06 08:20:00.000 +0200");
            REQUIRE(entry2.at("who") == "testuser");
            REQUIRE(entry2.at("where") == "loc01");
            REQUIRE(entry2.at("process") == "proc1");
            REQUIRE(entry2.at("version") == "SW 1.3");
            REQUIRE(entry2.at("what") == "raw data processing\nline 2\nline 3");
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

TEST_CASE("JdxConverter maps Bruker specific JCAMP-DX", "[JdxConverter]")
{
    using namespace libjdx::api;
    using namespace libjdx::jdx::api;

    SECTION("Bruker $RELAX sections as child nodes")
    {
        auto converter = JdxConverter("resources/Bruker_specific_relax.jdx");
        auto rootNode = converter.read("/");

        REQUIRE("Bruker Relax Type NMR Spectrum" == rootNode.name);
        REQUIRE(6 == rootNode.parameters.size());
        REQUIRE(rootNode.data.empty());
        REQUIRE(5 == rootNode.childNodeNames.size());
        REQUIRE("file_name_1" == rootNode.childNodeNames.at(0));
        REQUIRE("file_name_2" == rootNode.childNodeNames.at(1));
        REQUIRE("Bruker specific parameters" == rootNode.childNodeNames.at(2));
        REQUIRE("Bruker specific parameters for F1"
                == rootNode.childNodeNames.at(3));
        REQUIRE("NMR SPECTRUM" == rootNode.childNodeNames.at(4));

        SECTION("Bruker $RELAX section file_name_1 content as parameter")
        {
            auto brukerRelaxSection = converter.read("/0");

            REQUIRE("file_name_1" == brukerRelaxSection.name);
            REQUIRE(brukerRelaxSection.data.empty());
            REQUIRE(brukerRelaxSection.childNodeNames.empty());
            REQUIRE(1 == brukerRelaxSection.parameters.size());

            auto [key, value] = brukerRelaxSection.parameters.at(0);
            REQUIRE(key.empty());
            REQUIRE("1.0\n0.0 1.0 2.0\n" == value);
        }

        SECTION("Bruker $RELAX section file_name_2 content as parameter")
        {
            auto brukerRelaxSection = converter.read("/1");

            REQUIRE("file_name_2" == brukerRelaxSection.name);
            REQUIRE(brukerRelaxSection.data.empty());
            REQUIRE(brukerRelaxSection.childNodeNames.empty());
            REQUIRE(1 == brukerRelaxSection.parameters.size());

            auto [key, value] = brukerRelaxSection.parameters.at(0);
            REQUIRE(key.empty());
            REQUIRE("##TITLE= Parameter file\n"
                    "##JCAMPDX= 5.0\n"
                    "$$ c:/nmr/data/somepath\n"
                    "##$BLKPA= (0..15)\n"
                    "3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 \n"
                    "##$BLKTR= (0..15)\n"
                    "3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 \n"
                    "##$DE1= 2\n"
                    "##END=\n"
                    == value);
        }

        SECTION("Bruker specific parameters as parameters")
        {
            auto brukerParametersSection = converter.read("/2");

            REQUIRE(
                "Bruker specific parameters" == brukerParametersSection.name);
            REQUIRE(brukerParametersSection.data.empty());
            REQUIRE(brukerParametersSection.childNodeNames.empty());
            REQUIRE(4 == brukerParametersSection.parameters.size());

            REQUIRE("$DU" == brukerParametersSection.parameters.at(0).key);
            REQUIRE("<C:/>" == brukerParametersSection.parameters.at(0).value);
            REQUIRE("$NAME" == brukerParametersSection.parameters.at(1).key);
            REQUIRE("<Jul11-2023>"
                    == brukerParametersSection.parameters.at(1).value);
            REQUIRE("$AQSEQ" == brukerParametersSection.parameters.at(2).key);
            REQUIRE("0" == brukerParametersSection.parameters.at(2).value);
            REQUIRE("$AQMOD" == brukerParametersSection.parameters.at(3).key);
            REQUIRE("3" == brukerParametersSection.parameters.at(3).value);
        }

        SECTION("Bruker specific parameters for F1 as parameters")
        {
            auto brukerParametersSection = converter.read("/3");

            REQUIRE("Bruker specific parameters for F1"
                    == brukerParametersSection.name);
            REQUIRE(brukerParametersSection.data.empty());
            REQUIRE(brukerParametersSection.childNodeNames.empty());
            REQUIRE(3 == brukerParametersSection.parameters.size());

            REQUIRE("$AMP" == brukerParametersSection.parameters.at(0).key);
            REQUIRE("(0..31)\n"
                    "100 100 100 100 100 100 100 100 100 100 100 100 100 100 "
                    "100 100 "
                    "100 100 \n"
                    "100 100 100 100 100 100 100 100 100 100 100 100 100 100 "
                    == brukerParametersSection.parameters.at(0).value);
            REQUIRE("$AQSEQ" == brukerParametersSection.parameters.at(1).key);
            REQUIRE("0" == brukerParametersSection.parameters.at(1).value);
            REQUIRE("$AQMOD" == brukerParametersSection.parameters.at(2).key);
            REQUIRE("2" == brukerParametersSection.parameters.at(2).value);
        }

        SECTION("raises error if $RELAX section is not leaf node")
        {
            REQUIRE_THROWS_WITH(converter.read("/0/0"),
                Catch::Matchers::Contains("illegal", Catch::CaseSensitive::No));
        }

        SECTION("raises error if Bruker specific parameters section is not "
                "leaf node")
        {
            REQUIRE_THROWS_WITH(converter.read("/2/0"),
                Catch::Matchers::Contains("illegal", Catch::CaseSensitive::No));
        }
    }
}
