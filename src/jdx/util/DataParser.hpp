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
#ifndef JDX_DATAPARSER_HPP
#define JDX_DATAPARSER_HPP

#include "io/TextReader.hpp"

#include <optional>
#include <string>
#include <vector>

namespace libjdx::jdx::util
{
/**
 * @brief Helper functions for parsing JCAMP-DX data.
 */
class DataParser
{
public:
    static std::vector<double> readXppYYData(io::TextReader& reader);
    static std::vector<std::pair<double, double>> readXyXyData(
        io::TextReader& reader);
    static std::pair<std::vector<double>, bool> readValues(
        std::string& encodedValues, bool isAsdf);

private:
    enum class TokenType
    {
        Affn,
        Sqz,
        Dif,
        Dup,
        Missing,
    };
    static std::pair<std::vector<double>, bool> readXppYYLine(
        std::string& line, const std::optional<double>& yValueCheck);
    static std::optional<std::string> nextToken(
        const std::string& line, size_t& pos, bool isAsdf);
    static TokenType toAffn(std::string& token);
    static bool isTokenDelimiter(
        const std::string& encodedValues, size_t index);
    static bool isTokenStart(
        const std::string& encodedValues, size_t index, bool isAsdf);
    static bool isExponentStart(
        const std::string& encodedValues, size_t index, bool isAsdf);
    static bool isAsciiDigit(char c);
    static bool isSqzDigit(char c);
    static bool isDifDigit(char c);
    static bool isDupDigit(char c);
    static bool isSqzDifDupDigit(char c);
    static std::optional<char> getAsciiDigitValue(char c);
    static std::optional<char> getSqzDigitValue(char c);
    static std::optional<char> getDifDigitValue(char c);
    static std::optional<char> getDupDigitValue(char c);
};
} // namespace libjdx::jdx

#endif // JDX_DATAPARSER_HPP
