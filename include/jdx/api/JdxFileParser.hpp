#ifndef LIBJDX_API_JDXFILEPARSER_HPP
#define LIBJDX_API_JDXFILEPARSER_HPP

#include "api/FileParser.hpp"
#include "api/Node.hpp"

namespace sciformats::jdx::api
{
class JdxFileParser : public sciformats::api::FileParser
{
public:
    bool isRecognized(const std::string& path) override;
    std::unique_ptr<sciformats::api::Node> parse(
        const std::string& path) override;

private:
};
} // sciformats::jdx::api

#endif // LIBJDX_API_JDXFILEPARSER_HPP
