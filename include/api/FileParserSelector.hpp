#ifndef API_FILEPARSERSELECTOR_HPP
#define API_FILEPARSERSELECTOR_HPP

#include "api/FileParser.hpp"
#include "api/Node.hpp"

#include <string>
#include <vector>

namespace sciformats::api
{
class FileParserSelector : public FileParser
{
public:
    explicit FileParserSelector(
        std::vector<std::shared_ptr<FileParser>> fileParsers);
    bool isRecognized(const std::string& path) override;
    std::unique_ptr<Node> parse(const std::string& path) override;

private:
    std::vector<std::shared_ptr<FileParser>> m_fileParsers;
};
} // sciformats::api

#endif
