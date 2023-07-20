#ifndef JDX_API_JDXCONVERTER_HPP
#define JDX_API_JDXCONVERTER_HPP

#include "api/Converter.hpp"
#include "api/Node.hpp"
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
    static sciformats::api::Node mapBlock(const Block& block);
    static sciformats::api::Node mapBrukerRelaxSection(
        const BrukerRelaxSection& section);
    static sciformats::api::Node mapBrukerSpecificParameters(
        const BrukerSpecificParameters& section);
    static sciformats::api::Node mapNTuples(
        const NTuples& nTuples, const std::vector<size_t>& nodeIndices);
    static sciformats::api::Node mapNTuplesPage(const Page& page);
    static std::vector<sciformats::api::Point2D> mapData(const Block& block);
    static std::vector<sciformats::api::Point2D> mapXyData(
        const std::vector<std::pair<double, double>>& xyData);
};
} // sciformats::jdx::api

#endif
