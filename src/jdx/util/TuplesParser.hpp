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
#ifndef JDX_TUPLESPARSER_HPP
#define JDX_TUPLESPARSER_HPP

#include "io/TextReader.hpp"

#include <map>
#include <optional>
#include <regex>

namespace sciformats::jdx::util
{
/**
 * @brief A parser base class for tuple parsers such as AUDIT TRAIL, PEAK
 * ASSIGNMENTS, and PEAK TABLE.
 */
class TuplesParser
{
protected:
    /**
     * @brief Base for parsing tuples.
     * @param variableList A variable list describing the tuples.
     * @param ldrName The name of the LDR.
     */
    explicit TuplesParser(std::string variableList, std::string ldrName);

    /**
     * @brief The variable list.
     * @return The variable list.
     */
    [[nodiscard]] const std::string& getVariableList() const;

    /**
     * @brief The LDR name.
     * @return The LDR name.
     */
    [[nodiscard]] const std::string& getLdrName() const;

    /**
     * @brief Extracts tokens from a tuple string.
     * @param tuple The tuple.
     * @param regex Regex extracting tokens into groups.
     * @param numTokens Number of groups (including 0 == full match) to extract.
     * @return The extracted tokens.
     */
    [[nodiscard]] std::vector<std::optional<std::string>> extractTokens(
        const std::string& tuple, const std::regex& regex,
        size_t numTokens) const;

    /**
     * @brief Checks for errors and throws if a condition is fulfilled.
     * @param varList Variable list.
     * @param errorMap Map of {varList, {error condition, error message}}.
     * @param ldrName Name of the LDR.
     */
    static void checkForErrors(const std::string& varList,
        const std::multimap<std::string, std::tuple<bool, std::string>>&
            errorMap,
        const std::string& ldrName);

    /**
     * @brief Parses token as double.
     * @param token The token string.
     * @return The double parsed from the token.
     */
    static double parseDoubleToken(const std::optional<std::string>& token);

    template<typename R>
    std::optional<R> next(
        const std::function<std::optional<std::string>()>& nextTuple,
        const std::function<R(const std::string&)>& create);

private:
    const std::string m_variableList;
    const std::string m_ldrName;

    static std::optional<std::string> extractToken(
        const std::smatch& matches, size_t index);
    [[nodiscard]] std::smatch match(
        const std::string& tuple, const std::regex& regex) const;
};

template<typename R>
std::optional<R> sciformats::jdx::util::TuplesParser::next(
    const std::function<std::optional<std::string>()>& nextTuple,
    const std::function<R(const std::string&)>& create)
{
    auto tuple = nextTuple();
    if (!tuple)
    {
        return std::nullopt;
    }
    auto nextObject = create(tuple.value());
    return nextObject;
}

}

#endif /* JDX_TUPLESPARSER_HPP */
