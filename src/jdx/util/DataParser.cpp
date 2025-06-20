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
#include "util/DataParser.hpp"
#include "jdx/ParseException.hpp"
#include "util/LdrUtils.hpp"
#include "util/StringUtils.hpp"

#include <cmath>
#include <limits>
#include <string>

std::vector<double> libjdx::jdx::util::DataParser::readXppYYData(
    io::TextReader& reader)
{
    static_assert(
        std::numeric_limits<double>::has_quiet_NaN, "No quiet NaN available.");

    // read (X++(Y..Y)) data
    // TODO: possible performance tweak: yValues.reserve(NPOINTS)
    std::vector<double> yValues;
    std::string line;
    std::streamoff pos = reader.tellg();
    std::optional<double> yValueCheck = std::nullopt;
    while (!util::isLdrStart(line = reader.readLine()))
    {
        // save position to move back if next readLine() encounters LDR start
        pos = reader.tellg();
        // pre-process line
        auto data = util::stripLineComment(line, true).first;
        // read Y values from line
        auto [lineYValues, isDifEncoded] = readXppYYLine(data, yValueCheck);
        if (yValueCheck.has_value())
        {
            // y value is duplicated in new line, trust new value
            yValues.pop_back();
        }
        // append line values to yValues
        yValues.insert(
            std::end(yValues), std::begin(lineYValues), std::end(lineYValues));
        // if last and second to last values are defined, use last as y check
        if (!isDifEncoded || lineYValues.empty()
            || (lineYValues.size() == 1 && std::isnan(lineYValues.back()))
            || (lineYValues.size() >= 2
                && (std::isnan(lineYValues.back())
                    || std::isnan(lineYValues[lineYValues.size() - 2]))))
        {
            yValueCheck = std::nullopt;
        }
        else
        {
            yValueCheck = lineYValues.back();
        }
    }
    // next LDR encountered => all data read => move back to start of next LDR
    reader.seekg(pos);

    return yValues;
}

std::vector<std::pair<double, double>>
libjdx::jdx::util::DataParser::readXyXyData(io::TextReader& reader)
{
    static_assert(
        std::numeric_limits<double>::has_quiet_NaN, "No quiet NaN available.");

    // read (XY..XY) data
    std::vector<std::pair<double, double>> xyValues;
    bool lastValueIsXOnly = false;
    std::string line;
    std::streamoff pos = reader.tellg();
    while (!util::isLdrStart(line = reader.readLine()))
    {
        // save position to move back if next readLine() encounters LDR start
        pos = reader.tellg();
        // pre-process line
        auto data = util::stripLineComment(line, true).first;
        // read xy values from line
        auto lineValues = readValues(data, false).first;
        // turn line values into pairs and append line values to xyValues
        for (auto value : lineValues)
        {
            if (lastValueIsXOnly)
            {
                // must be y value
                xyValues.back().second = value;
                lastValueIsXOnly = false;
                continue;
            }
            // must be x value
            if (std::isnan(value))
            {
                throw ParseException(
                    "NaN value encountered as x value in line: " + line);
            }
            std::pair<double, double> xyValue{
                value, std::numeric_limits<double>::quiet_NaN()};
            lastValueIsXOnly = true;
            xyValues.push_back(xyValue);
        }
    }
    // next LDR encountered => all data read => move back to start of next LDR
    reader.seekg(pos);

    if (lastValueIsXOnly)
    {
        // uneven number of single values
        throw ParseException("Uneven number of values for xy data "
                             "encountered. No y value for x value: "
                             + std::to_string(xyValues.back().first));
    }
    return xyValues;
}

// TODO: refactor to reduce complexity
std::pair<std::vector<double>, bool>
// NOLINTNEXTLINE(readability-function-cognitive-complexity)
libjdx::jdx::util::DataParser::readValues(
    std::string& encodedValues, bool isAsdf)
{
    // output
    std::vector<double> yValues{};
    bool difEncoded = false;
    // state
    // for DIF/DUP previousTokenValue not same as last yValues value
    std::optional<double> previousTokenValue{};
    TokenType previousTokenType = TokenType::Affn;
    // loop
    size_t index = 0;
    while (auto token = nextToken(encodedValues, index, isAsdf))
    {
        TokenType tokenType = toAffn(token.value());
        // it's not quite clear if DUP of DIF should also count as DIF encoded
        // Bruker seems to think so => apply same logic here
        difEncoded = tokenType == TokenType::Dif
                     || (difEncoded && tokenType == TokenType::Dup);

        // check for logical errors
        if ((tokenType == TokenType::Dup || tokenType == TokenType::Dif)
            && !previousTokenValue.has_value())
        {
            throw ParseException(
                tokenType == TokenType::Dup
                    ? std::string{"DUP"}
                    : std::string{"DIF"}
                          + " token without preceding token encountered in "
                            "sequence: "
                          + encodedValues);
        }
        if ((tokenType == TokenType::Dup && previousTokenValue.has_value()
                && previousTokenType == TokenType::Dup))
        {
            throw ParseException(
                "DUP token with preceding DUP token encountered in sequence: "
                + encodedValues);
        }

        // process token
        if (tokenType == TokenType::Missing)
        {
            // ?
            yValues.push_back(std::numeric_limits<double>::quiet_NaN());
            previousTokenValue = std::numeric_limits<double>::quiet_NaN();
        }
        else if (tokenType == TokenType::Dup)
        {
            auto numRepeats = std::stol(token.value());
            for (auto i{0}; i < numRepeats - 1; i++)
            {
                if (previousTokenType == TokenType::Dif)
                {
                    auto lastValue = yValues.back();
                    auto nextValue = lastValue + previousTokenValue.value();
                    yValues.push_back(nextValue);
                }
                else
                {
                    yValues.push_back(yValues.back());
                }
            }
            previousTokenValue = numRepeats;
        }
        else
        {
            // TODO: use std::from_chars from <charconv> header
            // for better performance when more compilers support it
            // see: https://en.cppreference.com/w/cpp/utility/from_chars
            // https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html
            auto value = std::stod(token.value());
            if (tokenType == TokenType::Dif)
            {
                if (previousTokenType == TokenType::Missing)
                {
                    throw ParseException("DIF token with preceding ? token "
                                         "encountered in sequence: "
                                         + encodedValues);
                }
                auto lastValue = yValues.back();
                auto nextValue = lastValue + value;
                yValues.push_back(nextValue);
            }
            else
            {
                yValues.push_back(value);
            }
            previousTokenValue = value;
        }
        previousTokenType = tokenType;
    }
    return {yValues, difEncoded};
}

std::pair<std::vector<double>, bool>
libjdx::jdx::util::DataParser::readXppYYLine(
    std::string& line, const std::optional<double>& yValueCheck)
{
    // read (X++(Y..Y)) data line
    auto [values, difEncoded] = readValues(line, true);
    if (!values.empty())
    {
        // remove initial x value (not required for (X++(Y..Y)) encoded data)
        values.erase(values.begin());
        // skip X value check
    }
    if (yValueCheck.has_value() && !values.empty())
    {
        // first y value is a duplicate, check if roughly the same
        if (fabs(values.front() - yValueCheck.value()) >= 1)
        {
            throw ParseException("Y value check failed in line: " + line);
        }
    }
    return {values, difEncoded};
}

std::optional<std::string> libjdx::jdx::util::DataParser::nextToken(
    const std::string& line, size_t& pos, bool isAsdf)
{
    // skip delimiters
    while (pos < line.size() && isTokenDelimiter(line, pos))
    {
        pos++;
    }
    if (pos >= line.size())
    {
        return std::nullopt;
    }
    if (!isTokenStart(line, pos, isAsdf))
    {
        throw ParseException("illegal sequence encountered in line \"" + line
                             + "\" at position: " + std::to_string(pos));
    }
    auto startIt = line.cbegin() + static_cast<long>(pos++);
    while (!isTokenDelimiter(line, pos) && !isTokenStart(line, pos, isAsdf))
    {
        ++pos;
    }
    auto endIt = line.cbegin() + static_cast<long>(pos);
    return std::string{startIt, endIt};
}

libjdx::jdx::util::DataParser::TokenType libjdx::jdx::util::DataParser::toAffn(
    std::string& token)
{
    auto c = token.front();
    TokenType tokenType = TokenType::Affn;
    std::optional<char> firstDigit;
    if (c == '?')
    {
        tokenType = TokenType::Missing;
    }
    else if (isSqzDigit(c))
    {
        firstDigit = getSqzDigitValue(c);
        tokenType = TokenType::Sqz;
    }
    else if (isDifDigit(c))
    {
        firstDigit = getDifDigitValue(c);
        tokenType = TokenType::Dif;
    }
    else if (isDupDigit(c))
    {
        firstDigit = getDupDigitValue(c);
        tokenType = TokenType::Dup;
    }

    if (TokenType::Affn != tokenType && TokenType::Missing != tokenType)
    {
        // replace SQZ/DIF/DUP char (first char) with (signed) value
        // the below code should be faster than the naive:
        // token.erase(0, 1);
        // token.insert(0, std::to_string(firstDigit.value()));
        const char value = firstDigit.value();
        const char digit = value >= 0 ? static_cast<char>('0' + value)
                                      : static_cast<char>('0' - value);
        token[0] = digit;
        if (value < 0)
        {
            // TODO: possibly return +/- as separate bool to avoid insert()
            token.insert(0, "-");
        }
    }
    // must be plain AFFN or PAC (or illegal)
    return tokenType;
}

bool libjdx::jdx::util::DataParser::isTokenDelimiter(
    const std::string& encodedValues, size_t index)
{
    if (index >= encodedValues.size())
    {
        return true;
    }
    char c = encodedValues[index];
    return isSpace(c) || c == ';' || c == ',';
}

bool libjdx::jdx::util::DataParser::isTokenStart(
    const std::string& encodedValues, size_t index, bool isAsdf)
{
    if (index >= encodedValues.size())
    {
        return false;
    }
    char c = encodedValues[index];
    if ((isAsciiDigit(c) || c == '.')
        && (index == 0 || isTokenDelimiter(encodedValues, index - 1)))
    {
        return true;
    }
    if (c == 'E' || c == 'e')
    {
        // could be either an exponent or SQZ digit (E==+5, e==-5)
        // apply heuristic to provide answer
        return !isExponentStart(encodedValues, index, isAsdf);
    }
    if (c == '+' || c == '-')
    {
        // could be either a sign of an exponent or AFFN/PAC start digit
        // apply heuristic to provide answer
        if (index == 0)
        {
            return true;
        }
        return !isExponentStart(encodedValues, index - 1, isAsdf);
    }
    if (isSqzDifDupDigit(c))
    {
        return true;
    }
    if (c == '?')
    {
        // "invalid" data symbol
        return true;
    }
    return false;
}

bool libjdx::jdx::util::DataParser::isExponentStart(
    const std::string& encodedValues, size_t index, bool isAsdf)
{
    // a faster check for start of exponent instead of these regexes:
    // ^[eE][+-]{0,1}\\d{1,3}[;,\\s]{1}.*
    // ^[eE][+-]{0,1}\\d{1,3}[;,\\s]$
    // these heuristics are fragile
    if (index >= encodedValues.size())
    {
        throw ParseException(
            "Illegal index provided for exponent check. Line: " + encodedValues
            + ", index: " + std::to_string(index));
    }
    auto i = index;
    auto curChar = encodedValues[i++];
    if (curChar != 'E' && curChar != 'e')
    {
        return false;
    }
    if (i > encodedValues.size())
    {
        return false;
    }
    curChar = encodedValues[i++];
    const auto plusMinusFound = curChar == '+' || curChar == '-';
    if (!plusMinusFound && !isAsciiDigit(curChar))
    {
        return false;
    }
    // so far, e.g., "E+", "e-", "E2"
    if (plusMinusFound && i >= encodedValues.size())
    {
        // string ends with illegal sequence "E+" "E-", "e+", "e-"
        return false;
    }
    // no need to update curChar after this point
    auto numDigits = plusMinusFound ? 0 : 1;
    while (numDigits < 3 && i < encodedValues.size()
           && isAsciiDigit(encodedValues[i]))
    {
        ++i;
        ++numDigits;
    }
    // so far, e.g., "E+", "E+2", "e-34", "E234"
    if (numDigits == 0)
    {
        return false;
    }
    // compressed values can easily end in a line on, e.g., "E567", so require
    // delimiter
    if (isAsdf)
    {
        return i < encodedValues.size() && isTokenDelimiter(encodedValues, i);
    }
    // for AFFN
    return i >= encodedValues.size() || isTokenDelimiter(encodedValues, i);
}

bool libjdx::jdx::util::DataParser::isAsciiDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool libjdx::jdx::util::DataParser::isSqzDigit(char c)
{
    return (c >= '@' && c <= 'I') || (c >= 'a' && c <= 'i');
}

bool libjdx::jdx::util::DataParser::isDifDigit(char c)
{
    return (c >= 'J' && c <= 'R') || (c >= 'j' && c <= 'r') || c == '%';
}

bool libjdx::jdx::util::DataParser::isDupDigit(char c)
{
    return (c >= 'S' && c <= 'Z') || c == 's';
}

bool libjdx::jdx::util::DataParser::isSqzDifDupDigit(char c)
{
    return (c >= '@' && c <= 'Z') || (c >= 'a' && c <= 's') || c == '%';
}

std::optional<char> libjdx::jdx::util::DataParser::getAsciiDigitValue(char c)
{
    static const std::string asciiDigits = "0123456789";
    auto pos = asciiDigits.find(c);
    return pos == std::string::npos
               ? std::nullopt
               : std::make_optional(static_cast<char>(pos));
}

std::optional<char> libjdx::jdx::util::DataParser::getSqzDigitValue(char c)
{
    // positive SQZ digits @ABCDEFGHI
    if (c >= '@' && c <= 'I')
    {
        return c - '@';
    }
    // negative SQZ digits abcdefghi
    if (c >= 'a' && c <= 'i')
    {
        return '`' - c;
    }
    return {};
}

std::optional<char> libjdx::jdx::util::DataParser::getDifDigitValue(char c)
{
    // positive DIF digits %JKLMNOPQR
    if (c == '%')
    {
        return 0;
    }
    if (c >= 'J' && c <= 'R')
    {
        return c - 'I';
    }
    // negative DIF digits jklmnopqr
    if (c >= 'j' && c <= 'r')
    {
        return 'i' - c;
    }
    return {};
}

std::optional<char> libjdx::jdx::util::DataParser::getDupDigitValue(char c)
{
    // DUP digits STUVWXYZs
    if (c >= 'S' && c <= 'Z')
    {
        return c - 'R';
    }
    if (c == 's')
    {
        return 9;
    }
    return {};
}
