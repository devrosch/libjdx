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
#ifndef JDX_TABULARDATA_HPP
#define JDX_TABULARDATA_HPP

#include "io/TextReader.hpp"
#include "jdx/DataLdr.hpp"

#include <functional>
#include <regex>
#include <string>

namespace sciformats::jdx
{
/**
 * @brief Base class for JCAMP-DX PEAK TABLE and PEAK ASSIGNMENTS records.
 */
class TabularData : public DataLdr
{
public:
    /**
     * @brief Definition of peak width (and other kernel) functions.
     *
     * Comment $$ in line(s) following LDR start may contain peak width and
     * other peak kernel functions
     *
     * @return Textual description of peak width function.
     */
    [[nodiscard]] std::optional<std::string> getWidthFunction();

protected:
    TabularData(
        std::string label, std::string variableList, io::TextReader& reader);

    /**
     * @brief Provides the parsed peak assignments or peaks.
     * @param Parser for the data.
     * @return The list of peak assignments or peaks.
     */
    template<typename Parser, typename R>
    std::vector<R> getData(Parser parser) const;
};

template<typename Parser, typename R>
std::vector<R> sciformats::jdx::TabularData::getData(Parser parser) const
{
    const static std::regex pureCommentRegex{"^[ \t\n\v\f\r]*\\$\\$.*$"};
    auto func = [&]() {
        std::vector<R> data{};
        auto& reader = getReader();

        // TODO: use util::skipPureComments() but don't expose private header
        // skip possible initial comment lines
        std::optional<std::streampos> pos;
        while (!reader.eof())
        {
            pos = reader.tellg();
            auto line = reader.readLine();
            if (!std::regex_match(line, pureCommentRegex))
            {
                // not pure comment
                break;
            }
        }
        if (pos)
        {
            reader.seekg(pos.value());
        }

        // read peaks
        while (auto next = parser.next())
        {
            data.push_back(std::move(next.value()));
        }
        return data;
    };

    return callAndResetStreamPos<std::vector<R>>(func);
}

} // namespace sciformats::jdx

#endif // JDX_TABULARDATA_HPP
