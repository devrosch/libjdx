#ifndef LIBAPI_FILEPARSER_HPP
#define LIBAPI_FILEPARSER_HPP

#include "api/Node.hpp"

#include <string>

namespace sciformats::api
{
class FileParser
{
public:
    virtual bool isRecognized(const std::string& path) = 0;
    virtual std::unique_ptr<Node> parse(const std::string& path) = 0;

    // https://stackoverflow.com/questions/26039907/does-rule-of-three-five-apply-to-inheritance-and-virtual-destructors
    FileParser() = default;
    FileParser(const FileParser& node) = default;
    FileParser& operator=(const FileParser& node) = default;
    FileParser(FileParser&&) = default;
    FileParser& operator=(FileParser&&) = default;
    virtual ~FileParser() = default;
};
} // sciformats::api

#endif // LIBAPI_FILEPARSER_HPP
