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
#ifndef JDX_BRUKERSPECIFICPARAMETERS_HPP
#define JDX_BRUKERSPECIFICPARAMETERS_HPP

#include "io/TextReader.hpp"
#include "jdx/LdrContainer.hpp"
#include "jdx/StringLdr.hpp"

#include <optional>
#include <string>
#include <vector>

namespace libjdx::jdx
{
/**
 * @brief A JCAMP-DX Bruker specific parameters section.
 * @details This section starts with:
 *
 * $$ Bruker specific parameters <optional additional text>
 * $$ --------------------------
 *
 * and ends when another Bruker specific section starts or the section end is
 * indicated by:
 *
 * $$ End of Bruker specific parameters
 * $$ ---------------------------------
 */
class BrukerSpecificParameters : private LdrContainer
{
public:
    /**
     * @brief Constructs BrukerSpecificParameters.
     * @param reader Text reader with JCAMP-DX data. The reader position is
     * assumed to be at the start of the second line of the record. The reader
     * is expected to exist for the lifetime of this object.
     * @param nextLine The first line of the record, i.e., the one containing
     * the section start text. Will contain the line following the record or
     * nullopt if the end of the reader has been reached.
     */
    BrukerSpecificParameters(
        io::TextReader& reader, std::optional<std::string>& nextLine);

    /**
     * @brief Provides the name of the section.
     * @return The section name.
     */
    [[nodiscard]] std::string getName() const;

    /**
     * @brief Provides the contents of the section as LDRs.
     * @return The section content.
     */
    [[nodiscard]] std::vector<StringLdr> getContent() const;

private:
    static constexpr const char* s_sectionEndText
        = "$$ End of Bruker specific parameters";

    void parse(io::TextReader& reader, std::optional<std::string>& nextLine);
    static bool isDashedLine(std::optional<std::string>& nextLine);
    void parseLdrs(
        io::TextReader& reader, std::optional<std::string>& nextLine);

    std::string m_name;
    std::vector<StringLdr> m_content;
};
} // namespace libjdx::jdx

#endif // JDX_BRUKERSPECIFICPARAMETERS_HPP
