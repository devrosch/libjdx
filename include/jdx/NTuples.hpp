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
#ifndef JDX_NTUPLES_HPP
#define JDX_NTUPLES_HPP

#include "io/TextReader.hpp"
#include "jdx/LdrContainer.hpp"
#include "jdx/NTuplesAttributes.hpp"
#include "jdx/Page.hpp"
#include "jdx/StringLdr.hpp"

#include <functional>
#include <map>
#include <vector>

namespace libjdx::jdx
{
/**
 * @brief A JCAMP-DX NTUPLES record.
 */
class NTuples : public LdrContainer
{
public:
    /**
     * @brief Constructs the record.
     * @param label The label of the LDR, "NTUPLES".
     * @param dataForm The value of the first line of the record.
     * @param ldrs String LDRs of the surrounding block.
     * representing the data form, e.g. "NMR FID" or "MASS SPECTRUM".
     * @param reader Text reader with position assumed to be on the line
     * following the NTUPLES label.
     * @param nextLine The first line of the LDR, i.e. the one containing the
     * label. Will contain the line following the record or nullopt if the end
     * of the reader has been reached.
     */
    NTuples(const std::string& label, std::string dataForm,
        const std::vector<StringLdr>& blockLdrs, io::TextReader& reader,
        std::optional<std::string>& nextLine);

    /**
     * @brief The data form of the NTUPLES record (value of the
     * first line of the LDR), e.g., "NMR FID" or "MASS SPECTRUM".
     * @return The data form.
     */
    [[nodiscard]] std::string getDataForm() const;

    /**
     * @brief Returns the LDRs excluding PAGEs.
     * @return The LDRs in this record.
     */
    [[nodiscard]] const std::vector<StringLdr>& getLdrs() const;

    /**
     * @brief Returns the attributes parsed from the LDRs.
     * @return The page attributes.
     */
    [[nodiscard]] const std::vector<NTuplesAttributes>& getAttributes() const;

    /**
     * @brief Returns the number of pages in this record.
     * @return The number of pages.
     */
    [[nodiscard]] size_t getNumPages() const;

    /**
     * @brief Retrieves a page from the record.
     * @param pageIndex The page index starting at zero.
     * @return The page.
     */
    [[nodiscard]] const Page& getPage(size_t pageIndex) const;

private:
    static constexpr const char* s_label = "NTUPLES";
    static constexpr std::array<const char*, 11> s_standardAttrNames
        = {"VARNAME", "SYMBOL", "VARTYPE", "VARFORM", "VARDIM", "UNITS",
            "FIRST", "LAST", "MIN", "MAX", "FACTOR"};

    const std::string m_dataForm;
    std::vector<StringLdr> m_ldrs;
    std::vector<NTuplesAttributes> m_attributes;
    std::vector<Page> m_pages;

    static void validateInput(const std::string& label);
    void parse(const std::vector<StringLdr>& blockLdrs, io::TextReader& reader,
        std::optional<std::string>& nextLine);
    std::vector<NTuplesAttributes> parseAttributes(
        io::TextReader& reader, std::optional<std::string>& nextLine);
    static std::vector<StringLdr> readLdrs(
        std::optional<std::string>& firstLdrStart, io::TextReader& reader);
    static std::map<std::string, std::vector<std::string>> splitValues(
        const std::vector<StringLdr>& ldr);
    static std::map<std::string, std::vector<std::string>>
    extractStandardAttributes(
        std::map<std::string, std::vector<std::string>>& attributes);
    NTuplesAttributes map(const std::map<std::string, std::vector<std::string>>&
                              standardAttributes,
        const std::map<std::string, std::vector<std::string>>&
            additionalAttributes,
        size_t valueColumnIndex);
    static std::optional<std::vector<std::string>> findValue(
        const std::string& key,
        const std::map<std::string, std::vector<std::string>>& map);
};
} // namespace libjdx::jdx

#endif // JDX_NTUPLES_HPP
