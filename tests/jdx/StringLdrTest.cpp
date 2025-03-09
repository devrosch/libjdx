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
#include "jdx/StringLdr.hpp"

#include "catch2/catch.hpp"

TEST_CASE("LDR is initialized with two both arguments", "[StringLdr]")
{
    std::string label{"LABEL"};
    std::string value{"value"};

    auto ldr = libjdx::jdx::StringLdr{label, value};

    REQUIRE(label == ldr.getLabel());
    REQUIRE(value == ldr.getValue());
}

TEST_CASE("user defined LDRs are recognized", "[StringLdr]")
{
    auto standardLdr = libjdx::jdx::StringLdr{"TITLE", "value"};
    auto userDefinedLdr
        = libjdx::jdx::StringLdr{"$USER_DEFINED_LABEL", "user value"};
    auto techniqueSpecificLdr
        = libjdx::jdx::StringLdr{".OBSERVE_FREQUENCY", "50.0"};

    REQUIRE(false == standardLdr.isUserDefined());
    REQUIRE(true == userDefinedLdr.isUserDefined());
    REQUIRE(false == techniqueSpecificLdr.isUserDefined());
}

TEST_CASE("technique specific LDRs are recognized", "[StringLdr]")
{
    auto standardLdr = libjdx::jdx::StringLdr{"TITLE", "value"};
    auto userDefinedLdr
        = libjdx::jdx::StringLdr{"$USER_DEFINED_LABEL", "user value"};
    auto techniqueSpecificLdr
        = libjdx::jdx::StringLdr{".OBSERVE_FREQUENCY", "50.0"};

    REQUIRE(false == standardLdr.isTechniqueSpecific());
    REQUIRE(false == userDefinedLdr.isTechniqueSpecific());
    REQUIRE(true == techniqueSpecificLdr.isTechniqueSpecific());
}
