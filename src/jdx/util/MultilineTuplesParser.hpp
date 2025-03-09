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
#ifndef JDX_MULTILINETUPLESPARSER_HPP
#define JDX_MULTILINETUPLESPARSER_HPP

#include "io/TextReader.hpp"
#include "util/TuplesParser.hpp"

#include <optional>
#include <regex>

namespace sciformats::jdx::util
{
/**
 * @brief A parser base class for multiline tuple parsers such as AUDIT TRAIL
 * and PEAK ASSIGNMENTS.
 */
class MultilineTuplesParser : protected TuplesParser
{
protected:
    /**
     * @brief Base for parsing tuples that may span multiples lines, delimited
     * by a ")" at the tuples last line ends.
     * @param reader A reader at the start of the first tuple.
     * @param variableList A variable list describing the tuples.
     * @param ldrName The name of the LDR.
     * @param lineBreakChars A char array to replace line breaks with.
     */
    explicit MultilineTuplesParser(io::TextReader& reader,
        std::string variableList, std::string ldrName,
        std::string lineBreakChars);

    /**
     * @brief Retrieves the next tuple, delimited by ")" at the end of a line
     * from the io::TextReader.
     * @return The next tuple.
     */
    std::optional<std::string> nextTuple();

private:
    io::TextReader& m_reader;
    const std::string m_lineBreakChars;

    static bool isTupleStart(const std::string& stringValue);
    static bool isTupleEnd(const std::string& stringValue);
};
}

#endif /* JDX_MULTILINETUPLESPARSER_HPP */
