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
#include "io/BufferedTextReader.hpp"

#include <algorithm>
#include <fstream>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <vector>

libjdx::io::BufferedTextReader::BufferedTextReader(
    std::unique_ptr<std::istream> streamPtr, size_t bufferSize)
    : m_streamPtr{std::move(streamPtr)}
    , m_bufferMaxSize{bufferSize}
    , m_buffer(bufferSize)
    , m_bufferBasePos{0}
    , m_bufferPosIt{m_buffer.begin()}
{
    setStreamFlags();
    updateBuffer(0);
}

libjdx::io::BufferedTextReader::BufferedTextReader(
    const std::string& filePath, size_t bufferSize)
    : m_streamPtr{std::make_unique<std::ifstream>(filePath)}
    , m_bufferMaxSize{bufferSize}
    , m_buffer(bufferSize)
    , m_bufferBasePos{0}
    , m_bufferPosIt{m_buffer.begin()}
{
    setStreamFlags();
    updateBuffer(0);
}

void libjdx::io::BufferedTextReader::setStreamFlags()
{
    if (m_streamPtr == nullptr)
    {
        throw std::runtime_error("Text reader input stream is null.");
    }
    // the underlying read() method sets failbit and eofbit at end of file, so
    // do not set std::ios::failbit std::ios::eofbit
    m_streamPtr->exceptions(std::ios::badbit);
}

std::ios::pos_type libjdx::io::BufferedTextReader::calculateAbsolutePosition(
    std::ios::pos_type position, std::ios_base::seekdir seekdir)
{
    // find absolute stream position
    std::streampos pos = 0;
    if (seekdir == std::ios_base::beg)
    {
        pos = position;
    }
    else if (seekdir == std::ios_base::cur)
    {
        pos = m_bufferBasePos
              + static_cast<std::ios::pos_type>(
                  std::distance(m_buffer.cbegin(), m_bufferPosIt))
              + position;
    }
    else if (seekdir == std::ios_base::end)
    {
        pos = getLength() - position;
    }
    return pos;
}

void libjdx::io::BufferedTextReader::updateBuffer(std::ios::pos_type position)
{
    // NOLINTBEGIN(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
    auto bufferStartPos = static_cast<std::ios::pos_type>(
        (position / m_bufferMaxSize) * m_bufferMaxSize);
    m_streamPtr->seekg(bufferStartPos);
    m_buffer.resize(m_bufferMaxSize);
    m_streamPtr->read(m_buffer.data(), m_bufferMaxSize);
    const auto numCharsRead = m_streamPtr->gcount();
    if (m_streamPtr->eof())
    {
        m_streamPtr->clear();
    }
    m_buffer.resize(numCharsRead);
    m_bufferBasePos = bufferStartPos;
    m_bufferPosIt = std::begin(m_buffer)
                    + static_cast<std::vector<char>::difference_type>(
                        position - bufferStartPos);
    // NOLINTEND(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
}

std::ios::pos_type libjdx::io::BufferedTextReader::tellg() const
{
    return m_bufferBasePos
           + static_cast<std::ios::pos_type>(
               std::distance(m_buffer.cbegin(), m_bufferPosIt));
}

void libjdx::io::BufferedTextReader::seekg(
    std::ios::off_type position, std::ios_base::seekdir seekdir)
{
    auto pos = calculateAbsolutePosition(position, seekdir);
    // NOLINTBEGIN(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
    if (pos >= m_bufferBasePos
        && pos < m_bufferBasePos
                     + static_cast<std::ios::pos_type>(m_buffer.size()))
    // NOLINTEND(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
    {
        // new pos inside existing buffer => only update m_bufferPosIt
        auto newBufferPos = position - m_bufferBasePos;
        m_bufferPosIt
            = m_buffer.cbegin()
              + static_cast<std::vector<char>::difference_type>(newBufferPos);
    }
    else
    {
        const auto bufferEndPos
            = m_bufferBasePos
              + static_cast<std::ios::pos_type>(
                  std::distance(m_buffer.cbegin(), m_buffer.cend()));
        if (bufferEndPos == pos && pos == getLength())
        {
            // buffer already at end of stream => noop
            m_bufferPosIt = m_buffer.cend();
        }
        else
        {
            // new pos outside existing buffer => read chunk around new pos from
            // stream
            updateBuffer(pos);
        }
    }
}

std::ios::pos_type libjdx::io::BufferedTextReader::getLength()
{
    const std::ios::pos_type current = m_streamPtr->tellg();
    m_streamPtr->seekg(0, std::ios::end);
    const std::ios::pos_type length = m_streamPtr->tellg();
    m_streamPtr->seekg(current, std::ios::beg);
    return length;
}

bool libjdx::io::BufferedTextReader::eof() const
{
    // static cast is safe as distance from the beginning cannot benegative
    if (static_cast<size_t>(std::distance(m_buffer.cbegin(), m_bufferPosIt))
        < m_buffer.size())
    {
        return false;
    }
    // see:
    // https://stackoverflow.com/questions/6283632/how-to-know-if-the-next-character-is-eof-in-c
    auto c = m_streamPtr->peek();
    if (c == EOF)
    {
        if (m_streamPtr->eof())
        {
            // otherwise operations no longer works after eofbit is set
            m_streamPtr->clear();
            return true;
        }
        throw std::runtime_error(
            "End of file reached, but std::ios::eofbit not set.");
    }
    return false;
}

std::string libjdx::io::BufferedTextReader::readLine()
{
    // NOLINTBEGIN(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
    auto nextChunkStartPos
        = m_bufferBasePos + static_cast<std::ios::pos_type>(m_bufferMaxSize);
    if (m_bufferPosIt == m_buffer.cend() && nextChunkStartPos >= getLength())
    {
        throw std::runtime_error("Error reading line from istream.");
    }
    auto posIt = std::find(m_bufferPosIt, m_buffer.cend(), '\n');
    std::string out{m_bufferPosIt, posIt};
    auto lfFound = posIt != m_buffer.cend();
    // set new buffer position either past end or past found LF
    m_bufferPosIt = lfFound ? ++posIt : m_buffer.cend();
    while (!lfFound && nextChunkStartPos < getLength())
    {
        // no LF encountered => load next chunk if available and continue search
        updateBuffer(nextChunkStartPos);
        nextChunkStartPos = m_bufferBasePos
                            + static_cast<std::ios::pos_type>(m_bufferMaxSize);
        posIt = std::find(m_bufferPosIt, m_buffer.cend(), '\n');
        out.append(m_bufferPosIt, posIt);
        lfFound = posIt != m_buffer.cend();
        // set new buffer position either past end or past found LF
        m_bufferPosIt = lfFound ? ++posIt : m_buffer.cend();
    }
    // NOLINTEND(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
    if (m_streamPtr->eof())
    {
        // cleat eofbit, so that other operations will still succeed
        // other types of errors raise exceptions
        m_streamPtr->clear();
    }
    if (!out.empty() && out.at(out.size() - 1) == '\r')
    {
        // remove trailing \r in case line ending is \r\n and has not been
        // converted to \n by stream already
        out.erase(out.size() - 1);
    }
    return out;
}
