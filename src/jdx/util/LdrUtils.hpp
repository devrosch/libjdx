#ifndef JDX_LDRUTILS_HPP
#define JDX_LDRUTILS_HPP

#include "io/TextReader.hpp"
#include "util/StringUtils.hpp"
#include "jdx/StringLdr.hpp"

#include <optional>
#include <string>
#include <type_traits>
#include <vector>

/**
 * @brief Helper functions for parsing JCAMP-DX labeled data records (LDRs).
 */
namespace sciformats::jdx::util
{
bool isLdrStart(const std::string& line);
std::string normalizeLdrStart(const std::string& ldr);
std::string normalizeLdrLabel(const std::string& label);
std::pair<std::string, std::string> parseLdrStart(const std::string& ldrStart);
std::pair<std::string, std::optional<std::string>> stripLineComment(
    const std::string& line, bool trimContent = false,
    bool trimComment = false);
std::optional<const StringLdr> findLdr(
    const std::vector<StringLdr>& ldrs, const std::string& label);
std::optional<std::string> findLdrValue(
    const std::vector<StringLdr>& ldrs, const std::string& label);
void skipToNextLdr(io::TextReader& reader, std::optional<std::string>& nextLine,
    bool forceSkipFirstLine);
void skipPureComments(io::TextReader& reader,
    std::optional<std::string>& nextLine, bool mustPrecedeLdr);
bool isPureComment(const std::string& line);

template<typename T>
struct LdrValueParser
{
    // for std::string
    static std::optional<T> parse(const std::string& value)
    {
        // see: https://stackoverflow.com/questions/49330687/how-do-i-restrict-templates-to-specific-types
        // https://stackoverflow.com/questions/44847559/is-stdis-samet-tvalue-always-true
        static_assert(std::is_same_v<T, std::string>, "LdrValueParser template for string used with wrong type.");
        // could be empty string which is also a valid string
        return value;
    };
};
template<>
struct LdrValueParser<double>
{
    static std::optional<double> parse(const std::string& value)
    {
        return value.empty() ? std::nullopt : std::optional<double>{std::stod(value)};
    };
};
template<>
struct LdrValueParser<uint64_t>
{
    static std::optional<uint64_t> parse(const std::string& value)
    {
        // we're parsing uint64_t as unsigned long and assigning to uint64_t
        static_assert(std::numeric_limits<unsigned long>::max()
            // NOLINTNEXTLINE(misc-redundant-expression)
            <= std::numeric_limits<uint64_t>::max(),
            "unsigned long max larger than uint_64_t max");
        return value.empty() ? std::nullopt : std::optional<uint64_t>{std::stoul(value)};
    };
};

template <typename T>
std::optional<T> parseLdrValue(const std::vector<StringLdr>& ldrs, const std::string& label) {
    auto stringValueOptional = findLdrValue(ldrs, label);
    if (!stringValueOptional.has_value())
    {
        // LDR not found
        return std::nullopt;
    }
    auto [stringValue, _] = stripLineComment(stringValueOptional.value(), true);
    return LdrValueParser<T>::parse(stringValue);
}
} // namespace sciformats::jdx::utils

#endif
