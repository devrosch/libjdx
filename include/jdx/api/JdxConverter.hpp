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

namespace sciformats::jdx::api
{
class JdxConverter : public sciformats::api::Converter
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
    sciformats::api::Node read(const std::string& path) override;

private:
    std::unique_ptr<sciformats::jdx::Block> m_rootBlock;

    static std::vector<size_t> convertPathToNodeIndices(
        const std::string& path);
    sciformats::api::Node retrieveNode(const std::vector<size_t>& nodeIndices);
    static sciformats::api::Node mapBlock(const Block& block, bool isPeakData);
    static bool isPeakData(const Block& block);
    static sciformats::api::Node mapBrukerRelaxSection(
        const BrukerRelaxSection& section);
    static sciformats::api::Node mapBrukerSpecificParameters(
        const BrukerSpecificParameters& section);
    static sciformats::api::Node mapNTuples(const NTuples& nTuples,
        const std::vector<size_t>& nodeIndices, bool isPeakData);
    static sciformats::api::Node mapNTuplesPage(
        const Page& page, bool isPeakData);
    static std::string mapNTuplesPageName(const Page& page);
    static std::vector<sciformats::api::Point2D> mapData(const Block& block);
    static std::vector<sciformats::api::Point2D> mapXyData(
        const std::vector<std::pair<double, double>>& xyData);
    static std::map<std::string, std::string> mapMetadata(const Block& block);
    static std::map<std::string, std::string> mapMetadata(
        const Page& page, bool isPeakData);
    static sciformats::api::Table mapPeakTable(
        const sciformats::jdx::PeakTable& peakTable);
    static std::vector<sciformats::api::Point2D> mapPeakTableAsData(
        const sciformats::jdx::PeakTable& peakTable);
    static sciformats::api::Table mapDataAsPeakTable(
        const std::vector<std::pair<double, double>>& xyData);
    static sciformats::api::Table mapPeakAssignments(
        const sciformats::jdx::PeakAssignments& peakAssignments);
    static sciformats::api::Node mapAuditTrail(
        const sciformats::jdx::AuditTrail& auditTrail);
};
} // sciformats::jdx::api

#endif
