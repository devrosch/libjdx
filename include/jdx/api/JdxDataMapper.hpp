#ifndef JDX_API_JDXDATAMAPPER_HPP
#define JDX_API_JDXDATAMAPPER_HPP

#include "api/DataMapper.hpp"
#include "api/Node2.hpp"
#include "jdx/Block.hpp"

namespace sciformats::jdx::api
{
class JdxDataMapper : public sciformats::api::DataMapper
{
public:
    /**
     * Constructs a JdxDataMapper from a path.
     * @param path Path to the data set.
     */
    explicit JdxDataMapper(const std::string& path);

    /**
     * @brief Read node from data set.
     * @param path Path to the node within the data set.
     */
    sciformats::api::Node2 read(const std::string& path) override;

private:
    static constexpr std::array<const char*, 11> s_nTuplesStandardAttrNames
        = {"VAR_NAME", "SYMBOL", "VAR_TYPE", "VAR_FORM", "VAR_DIM", "UNITS",
            "FIRST", "LAST", "MIN", "MAX", "FACTOR"};

    std::unique_ptr<sciformats::jdx::Block> m_rootBlock;

    static std::vector<size_t> convertPathToNodeIndices(
        const std::string& path);
    sciformats::api::Node2 retrieveNode(const std::vector<size_t>& nodeIndices);
    static sciformats::api::Node2 mapBlock(const Block& block);
    static sciformats::api::Node2 mapNTuples(const NTuples& nTuples,
        const std::vector<size_t>& nodeIndices);
    static sciformats::api::Node2 mapNTuplesPage(const Page& page);
    static std::vector<sciformats::api::KeyValueParam> mapNTuplesAttributes(
        const NTuplesAttributes& nTuplesAttributes);
    static void mergeAttributes(
        std::vector<sciformats::api::KeyValueParam>& parameters,
        const NTuplesAttributes& nTuplesAttributes, size_t colIndex);
    static std::optional<std::string> findAttribute(
        const NTuplesAttributes& nTuplesAttributes, const std::string& key);
    static std::vector<sciformats::api::Point2D> mapData(const Block& block);
    static std::vector<sciformats::api::Point2D> mapXyData(
        const std::vector<std::pair<double, double>>& xyData);
};
} // sciformats::jdx::api

#endif
