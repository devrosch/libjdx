#ifndef LIBSCIWRAP_STUBFILEPARSER_HPP
#define LIBSCIWRAP_STUBFILEPARSER_HPP

#include "api/FileParser.hpp"
#include "api/Node.hpp"

namespace sciformats::stub
{
class StubFileParser : public sciformats::api::FileParser
{
public:
    bool isRecognized(const std::string& path) override;
    std::unique_ptr<sciformats::api::Node> parse(const std::string& path) override;

private:
};
} // sciformats::stub

#endif // LIBSCIWRAP_STUBFILEPARSER_HPP
