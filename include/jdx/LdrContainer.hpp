#ifndef JDX_LDRCONTAINER_HPP
#define JDX_LDRCONTAINER_HPP

#include "io/TextReader.hpp"

#include <optional>

namespace sciformats::jdx
{
/**
 * @brief Parent class of JCAMP-DX BLOCK and NTUPLES records.
 */
class LdrContainer
{
protected:
    static std::optional<const std::string> parseStringValue(
        std::string& value, io::TextReader& reader);
};
} // namespace sciformats::jdx

#endif // JDX_LDRCONTAINER_HPP
