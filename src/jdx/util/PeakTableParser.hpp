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
#ifndef JDX_PEAKTABLEPARSER_HPP
#define JDX_PEAKTABLEPARSER_HPP

#include "io/TextReader.hpp"
#include "jdx/PeakTable.hpp"
#include "util/TuplesParser.hpp"

#include <array>
#include <optional>
#include <queue>
#include <regex>

namespace libjdx::jdx::util
{
/**
 * @brief A parser for PEAK TABLE.
 */
class PeakTableParser : protected TuplesParser
{
public:
    explicit PeakTableParser(io::TextReader& reader, std::string variableList);

    /**
     * @brief Next peak.
     * @note Assumes that a peak tuple does not span multiple lines, but one
     * line may contain multiple tuples.
     * @return The next peak, nullopt if there is none.
     * @throws ParseException If there the next peak is malformed.
     */
    std::optional<Peak> next();

private:
    static constexpr const char* s_ldrName = "peak table";

    static constexpr std::array<const char*, 3> s_varLists = {
        "(XY..XY)",
        "(XYW..XYW)",
        "(XYM..XYM)",
    };

    /**
     * matches 2-3 peak segments as groups 1-3, corresponding to
     * (XY..XY), (XYW..XYW), or (XYM..XYM), with X as matches[1], Y as matche[2]
     * and W or M as matches[3]
     */
    const std::regex m_regex{R"(^\s*)"
                             R"(([^,]*))"
                             R"((?:\s*,\s*([^,]*)))"
                             R"((?:\s*,\s*([^,]*))?)"
                             R"($)"};

    io::TextReader& m_reader;
    std::queue<std::string> m_tuples;

    // tuple
    std::optional<std::string> nextTuple();
    // peak
    [[nodiscard]] libjdx::jdx::Peak createPeak(const std::string& tuple) const;
};
}

#endif /* JDX_PEAKTABLEPARSER_HPP */
