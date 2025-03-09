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
#ifndef JDX_API_JDXCONVERTER_HPP
#define JDX_API_JDXCONVERTER_HPP

#include "api/Converter.hpp"
#include "api/Node.hpp"
#include "api/Table.hpp"
#include "jdx/Block.hpp"

namespace libjdx::jdx::api
{
class JdxConverter : public libjdx::api::Converter
{
public:
    /**
     * Constructs a JdxConverter from a path.
     * @param path Path to the data set.
     */
    explicit JdxConverter(const std::string& path);

    /**
     * @brief Read node from data set.
     * @param path Path to the node within the data set.
     */
    libjdx::api::Node read(const std::string& path) override;

private:
    std::unique_ptr<libjdx::jdx::Block> m_rootBlock;

    static std::vector<size_t> convertPathToNodeIndices(
        const std::string& path);
    libjdx::api::Node retrieveNode(const std::vector<size_t>& nodeIndices);
    static libjdx::api::Node mapBlock(const Block& block, bool isPeakData);
    static bool isPeakData(const Block& block);
    static libjdx::api::Node mapBrukerRelaxSection(
        const BrukerRelaxSection& section);
    static libjdx::api::Node mapBrukerSpecificParameters(
        const BrukerSpecificParameters& section);
    static libjdx::api::Node mapNTuples(const NTuples& nTuples,
        const std::vector<size_t>& nodeIndices, bool isPeakData);
    static libjdx::api::Node mapNTuplesPage(const Page& page, bool isPeakData);
    static std::string mapNTuplesPageName(const Page& page);
    static std::vector<libjdx::api::Point2D> mapData(const Block& block);
    static std::vector<libjdx::api::Point2D> mapXyData(
        const std::vector<std::pair<double, double>>& xyData);
    static std::map<std::string, std::string> mapMetadata(const Block& block);
    static std::map<std::string, std::string> mapMetadata(
        const Page& page, bool isPeakData);
    static libjdx::api::Table mapPeakTable(
        const libjdx::jdx::PeakTable& peakTable);
    static std::vector<libjdx::api::Point2D> mapPeakTableAsData(
        const libjdx::jdx::PeakTable& peakTable);
    static libjdx::api::Table mapDataAsPeakTable(
        const std::vector<std::pair<double, double>>& xyData);
    static libjdx::api::Table mapPeakAssignments(
        const libjdx::jdx::PeakAssignments& peakAssignments);
    static libjdx::api::Node mapAuditTrail(
        const libjdx::jdx::AuditTrail& auditTrail);
};
} // libjdx::jdx::api

#endif
