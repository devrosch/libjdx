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
#ifndef JDX_AUDITTRAILPARSER_HPP
#define JDX_AUDITTRAILPARSER_HPP

#include "io/TextReader.hpp"
#include "jdx/AuditTrailEntry.hpp"
#include "util/MultilineTuplesParser.hpp"

#include <array>
#include <optional>

namespace libjdx::jdx::util
{
/**
 * @brief A parser for AUDIT TRAIL.
 */
class AuditTrailParser : protected MultilineTuplesParser
{
public:
    explicit AuditTrailParser(io::TextReader& reader, std::string variableList);

    /**
     * @brief Next audit trail entry.
     * @note Assumes that an audit trail entry tuple always starts on a new
     * line, but may span multiple lines.
     * @return The next audit trail entry, nullopt if there is none.
     * @throws ParseException If next audit trail entry is malformed.
     */
    std::optional<AuditTrailEntry> next();

private:
    static constexpr const char* s_ldrName = "audit trail";

    static constexpr std::array<const char*, 3> s_varLists = {
        "(NUMBER, WHEN, WHO, WHERE, WHAT)",
        "(NUMBER, WHEN, WHO, WHERE, VERSION, WHAT)",
        "(NUMBER, WHEN, WHO, WHERE, PROCESS, VERSION, WHAT)",
    };

    /**
     * matches 5 - 7 audit trail entry segments as groups 1-7, groups 5 nd 6
     * being optional, corresponding to one of (NUMBER, WHEN, WHO, WHERE, WHAT),
     * (NUMBER, WHEN, WHO, WHERE, VERSION, WHAT),
     * (NUMBER, WHEN, WHO, WHERE, PROCESS, VERSION, WHAT)
     */
    const std::regex m_regex{R"(^\s*\(\s*)"
                             R"((\d))"
                             R"((?:\s*,\s*<([^>]*)>))"
                             R"((?:\s*,\s*<([^>]*)>))"
                             R"((?:\s*,\s*<([^>]*)>))"
                             R"((?:\s*,\s*<([^>]*)>)?)"
                             R"((?:\s*,\s*<([^>]*)>)?)"
                             R"((?:\s*,\s*<([^>]*)>))"
                             R"(\s*\)\s*$)"};

    [[nodiscard]] libjdx::jdx::AuditTrailEntry createAuditTrailEntry(
        const std::string& tuple) const;
};
}

#endif /* JDX_AUDITTRAILPARSER_HPP */
