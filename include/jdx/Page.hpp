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
#ifndef JDX_PAGE_HPP
#define JDX_PAGE_HPP

#include "io/TextReader.hpp"
#include "jdx/DataTable.hpp"
#include "jdx/LdrContainer.hpp"
#include "jdx/NTuplesAttributes.hpp"
#include "jdx/StringLdr.hpp"

#include <array>
#include <functional>
#include <map>
#include <vector>

namespace libjdx::jdx
{
/**
 * @brief A JCAMP-DX NTUPLES PAGE record.
 */
class Page : LdrContainer
{
public:
    /**
     * @brief Constructs the record.
     * @param label he label of the LDR, "PAGE".
     * @param pageVar The PAGE variables, e.g., "N=1".
     * @param nTuplesAttributes The attributes of the surrounding NTUPLES
     * record.
     * @param blockLdrs The LDRs of the surrounding block.
     * @param reader Text reader with position assumed to be on the line
     * following the "PAGE" label.
     * @param nextLine The first line of the LDR, i.e. the one containing the
     * label. Will contain the line following the record or nullopt if the end
     * of the reader has been reached.
     */
    Page(std::string& label, std::string pageVar,
        const std::vector<NTuplesAttributes>& nTuplesAttributes,
        const std::vector<StringLdr>& blockLdrs, io::TextReader& reader,
        std::optional<std::string>& nextLine);

    /**
     * @brief The page variables of the PAGE record (value of
     * the first line of the LDR), e.g., "N=1" or "X=2.2, Y=3.3".
     * @return The page variables.
     */
    [[nodiscard]] const std::string& getPageVariables() const;

    /**
     * @brief The LDRs contained by the PAGE, e.g.
     * "NPOINTS", not including "DATA TABLE".
     * @return The page LDRs.
     */
    [[nodiscard]] const std::vector<StringLdr>& getPageLdrs() const;

    /**
     * @brief The DATA TABLE.
     * @return The DATA TABLE.
     */
    [[nodiscard]] const std::optional<DataTable>& getDataTable() const;

private:
    static constexpr const char* s_label = "PAGE";

    const std::string m_pageVariables;
    std::vector<StringLdr> m_pageLdrs;
    std::optional<DataTable> m_dataTable;

    static void validateInput(const std::string& label);
    void parse(const std::vector<NTuplesAttributes>& nTuplesAttributes,
        const std::vector<StringLdr>& blockLdrs, io::TextReader& reader,
        std::optional<std::string>& nextLine);
    static std::vector<StringLdr> parsePageLdrs(
        io::TextReader& reader, std::optional<std::string>& nextLine);
    static std::pair<std::string, std::optional<std::string>>
    parseDataTableVars(const std::string& rawPageVars);
};
} // namespace libjdx::jdx

#endif // JDX_PAGE_HPP
