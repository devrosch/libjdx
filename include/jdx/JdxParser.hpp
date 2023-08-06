#ifndef JDX_JDXPARSER_HPP
#define JDX_JDXPARSER_HPP

#include "jdx/Block.hpp"

#include <istream>
#include <memory>
#include <string>

namespace sciformats::jdx
{
/**
 * @brief A parser for JCAMP-DX data.
 */
class JdxParser
{
public:
    /**
     * @brief Shallow check if the data can be parsed, e.g. by checking the file
     * extension or magic bytes.
     * @param filePath Path to the file.
     * @param iStream Binary input stream.
     * @return A tentative indication whether this parser can process the file.
     */
    bool static canParse(const std::string& filePath, std::istream& iStream);

    /**
     * @brief Parses the data.
     * @param stream Binary input stream for the data.
     * @return A Block representing the data.
     */
    Block static parse(std::unique_ptr<std::istream> streamPtr);

    /**
     * @brief Parses the data.
     * @param filePath Path to the file.
     * @return A Block representing the data.
     */
    Block static parse(const std::string& filePath);

private:
    static constexpr std::array<const char*, 3> s_acceptedExtensions
        = {".jdx", ".dx", ".jcm"};
    static constexpr size_t s_scanBufferSize = 16;
};
} // namespace sciformats::jdx

#endif // JDX_JDXPARSER_HPP
