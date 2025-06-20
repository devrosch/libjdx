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
#ifndef JDX_BLOCK_HPP
#define JDX_BLOCK_HPP
#include "io/TextReader.hpp"
#include "jdx/AuditTrail.hpp"
#include "jdx/BlockParseException.hpp"
#include "jdx/BrukerRelaxSection.hpp"
#include "jdx/BrukerSpecificParameters.hpp"
#include "jdx/LdrContainer.hpp"
#include "jdx/NTuples.hpp"
#include "jdx/PeakAssignments.hpp"
#include "jdx/PeakTable.hpp"
#include "jdx/RaData.hpp"
#include "jdx/StringLdr.hpp"
#include "jdx/XyData.hpp"
#include "jdx/XyPoints.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace libjdx::jdx
{
/**
 * @brief A JCAMP-DX block. Can be a link or data block.
 */
class Block : public LdrContainer
{
public:
    /**
     * @brief Constructs a Block from text reader data.
     * @param readerPtr Text reader with JCAMP-DX data. The reader position
     * is assumed to be at the start of the first line of the block (containing
     * the TITLE LDR).
     */
    explicit Block(std::unique_ptr<io::TextReader> readerPtr);

    /**
     * @brief Constructs a Block from text reader data.
     * @param reader Text reader with JCAMP-DX data. The reader position
     * is assumed to be at the start of the first line of the block (containing
     * the TITLE LDR). The reader is expected to exist for the lifetime of
     * this object.
     */
    explicit Block(io::TextReader& reader);

    /**
     * @brief Provides the labeled data records (LDRs) of the Block.
     * This does \em not include the following LDRs:
     * - comments ("##=")
     * - data (XYDATA, XYPOINTS, PEAK TABLE, PEAK ASSIGNMENTS, RADATA, NTUPLES)
     *
     * Use the specialized member functions to retrieve the respective data.
     *
     * @return The LDRs in this block. The key is the label without "##" and "="
     * and the value is the content (without initial blank character if any).
     * E.g. the LDR "##TITLE= abc" has label "TITLE" and content "abc".
     */
    [[nodiscard]] const std::vector<StringLdr>& getLdrs() const;

    /**
     * @brief Provides a labeled data record (LDR) from the block. The same
     * exclusions as for getLdrs() apply.
     * @param label The label of the LDR. Search will use normalized form of
     * label, e.g. "Title" and "TI TLE" will both find the "TITLE" LDR.
     * @return The LDR for the given label if it exists in the block,
     * std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<const StringLdr> getLdr(
        const std::string& label) const;

    /**
     * @brief Provides the nested Blocks of the Block.
     * @return Blocks that are nested in this (LINK) block.
     */
    [[nodiscard]] const std::vector<Block>& getBlocks() const;

    /**
     * @brief Provides the labeled data records (LDRs) of the
     * Block that are comments (i.e. "##= <comment>").
     * @return The comment contents. The content of a comment is the text
     * following the "=" without initial blank character if any. E.g. the
     * comment "##= abc" has content "abc".
     */
    [[nodiscard]] const std::vector<std::string>& getLdrComments() const;

    /**
     * @brief Provides the XYDATA record if available.
     * @return XYDATA record.
     */
    [[nodiscard]] const std::optional<XyData>& getXyData() const;

    /**
     * @brief Provides the RADATA record if available.
     * @return RADATA record.
     */
    [[nodiscard]] const std::optional<RaData>& getRaData() const;

    /**
     * @brief Provides the XYPOINTS record if available.
     * @return XYPOINTS record.
     */
    [[nodiscard]] const std::optional<XyPoints>& getXyPoints() const;

    /**
     * @brief Provides the PEAK TABLE record if available.
     * @return PEAK TABLE record.
     */
    [[nodiscard]] const std::optional<PeakTable>& getPeakTable() const;

    /**
     * @brief Provides the PEAK ASSIGNMENTS record if available.
     * @return PEAK ASSIGNMENTS record.
     */
    [[nodiscard]] const std::optional<PeakAssignments>&
    getPeakAssignments() const;

    /**
     * @brief Provides the NTUPLES record if available.
     * @return NTUPLES record.
     */
    [[nodiscard]] const std::optional<NTuples>& getNTuples() const;

    /**
     * @brief Provides the AUDIT TRAIL record if available.
     * @return AUDIT TRAIL record.
     */
    [[nodiscard]] const std::optional<AuditTrail>& getAuditTrail() const;

    /**
     * @brief Provides data contained in Bruker specific parameter sections if
     * available.
     * @return Bruker specific records.
     */
    [[nodiscard]] const std::vector<BrukerSpecificParameters>&
    getBrukerSpecificParameters() const;

    /**
     * @brief Provides data contained in Bruker RELAX sections if available.
     * @return Bruker specific RELAX sections.
     */
    [[nodiscard]] const std::vector<BrukerRelaxSection>&
    getBrukerRelaxSections() const;

private:
    static constexpr const char* s_blockStartLabel = "TITLE";

    std::unique_ptr<io::TextReader> m_readerPtr;
    io::TextReader& m_reader;
    std::vector<StringLdr> m_ldrs;
    std::vector<std::string> m_ldrComments;
    std::vector<Block> m_blocks;
    std::optional<XyData> m_xyData;
    std::optional<RaData> m_raData;
    std::optional<XyPoints> m_xyPoints;
    std::optional<PeakTable> m_peakTable;
    std::optional<PeakAssignments> m_peakAssignments;
    std::optional<NTuples> m_nTuples;
    std::optional<AuditTrail> m_auditTrail;
    std::vector<BrukerSpecificParameters> m_brukerSpecificParameters;
    std::vector<BrukerRelaxSection> m_brukerRelaxSections;

    /**
     * @brief Constructs a Block from first line value and reader.
     * @param title The value of the first line of the block, i.e. the content
     * of the line following the "##TITLE=" label.
     * @param reader Text reader with JCAMP-DX data. The reader position
     * is assumed to be at the start of the second line (the line following the
     * TITLE line) of the block. The reader is expected to exist for the
     * lifetime of this object.
     * @param nextLine The first line of the LDR, i.e. the one containing the
     * label. Will contain the line following the record or nullopt if the end
     * of the reader has been reached.
     */
    Block(const std::string& title, io::TextReader& reader,
        std::optional<std::string>& nextLine);
    static std::string parseFirstLine(const std::string& firstLine);
    void parseInput(
        const std::string& title, std::optional<std::string>& nextLine);
    template<typename T>
    void addLdr(const std::string& title, const std::string& label,
        std::optional<T>& member, const std::function<T()>& builderFunc);
};

template<typename T>
void libjdx::jdx::Block::addLdr(const std::string& title,
    const std::string& label, std::optional<T>& member,
    const std::function<T()>& builderFunc)
{
    if (member)
    {
        // duplicate => error
        throw BlockParseException("Multiple", label, title);
    }
    member.emplace(builderFunc());
}
} // namespace libjdx::jdx

#endif // JDX_BLOCK_HPP
