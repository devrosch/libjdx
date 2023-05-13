#include "jdx/BufferedTextReader.hpp"
#include "jdx/ParseException.hpp"

#include <istream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

sciformats::jdx::BufferedTextReader::BufferedTextReader(std::unique_ptr<std::istream> streamPtr, size_t bufferSize)
    : m_streamPtr{std::move(streamPtr)}
    , m_bufferMaxSize{bufferSize}
    , m_buffer(bufferSize)
    , m_bufferBasePos{0}
    , m_bufferPosIt{m_buffer.begin()}
{
    setStreamFlags();
    updateChunk(0);
}

sciformats::jdx::BufferedTextReader::BufferedTextReader(const std::string& filePath, size_t bufferSize)
    : m_streamPtr{std::make_unique<std::ifstream>(filePath)}
    , m_bufferMaxSize{bufferSize}
    , m_buffer(bufferSize)
    , m_bufferBasePos{0}
    , m_bufferPosIt{m_buffer.begin()}
{
    setStreamFlags();
    updateChunk(0);
}

void sciformats::jdx::BufferedTextReader::setStreamFlags()
{
    if (m_streamPtr == nullptr)
    {
        throw ParseException("Text reader input stream is null.");
    }
    // the underlying read() method sets failbit and eofbit at end of file, so do not set
    // std::ios::eofbit
    m_streamPtr->exceptions(std::ios::badbit);
}

std::ios::pos_type sciformats::jdx::BufferedTextReader::calculateAbsolutePosition(std::ios::pos_type position, std::ios_base::seekdir seekdir)
{
    // find absolute stream position
    std::streampos pos = 0;
    if (seekdir == std::ios_base::beg)
    {
        pos = m_bufferBasePos + position;
    }
    else if (seekdir == std::ios_base::cur)
    {
        pos = m_bufferBasePos + static_cast<std::ios::pos_type>(std::distance(m_buffer.cbegin(), m_bufferPosIt)) + position;
    }
    else if (seekdir == std::ios_base::end)
    {
        pos = m_bufferBasePos - position;
    }
    return pos;
}

void sciformats::jdx::BufferedTextReader::updateChunk(std::ios::pos_type position)
{
    auto bufferStartPos = static_cast<std::ios::pos_type>((position / m_bufferMaxSize) * m_bufferMaxSize);
    auto good0 = m_streamPtr->good();
    m_streamPtr->seekg(bufferStartPos);
    auto good1 = m_streamPtr->good();
    m_streamPtr->read(m_buffer.data(), m_bufferMaxSize);
    auto good2 = m_streamPtr->good();
    const auto numCharsRead = m_streamPtr->gcount();
    if (m_streamPtr->eof())
    {
        m_streamPtr->clear();
    }
    auto good3 = m_streamPtr->good();
    m_buffer.resize(numCharsRead);
    m_bufferPosIt = std::begin(m_buffer) + (position - bufferStartPos);
}

std::ios::pos_type sciformats::jdx::BufferedTextReader::tellg() const
{
    return m_bufferBasePos + static_cast<std::ios::pos_type>(std::distance(m_buffer.cbegin(), m_bufferPosIt));
}

void sciformats::jdx::BufferedTextReader::seekg(
    std::ios::off_type position, std::ios_base::seekdir seekdir)
{
    auto pos = calculateAbsolutePosition(position, seekdir);
    if (pos >= m_bufferBasePos && pos < m_bufferBasePos + static_cast<std::ios::pos_type>(m_buffer.size()))
    {
        // new pos inside existing buffer => only update m_bufferPosIt
        auto newBufferPos = position - m_bufferBasePos;
        m_bufferPosIt = m_buffer.cbegin() + newBufferPos;
    }
    else {
        const auto bufferEndPos = m_bufferBasePos + static_cast<std::ios::pos_type>(std::distance(m_buffer.cbegin(), m_buffer.cend()));
        if (bufferEndPos == pos && pos == getLength())
        {
            // buffer already at end of stream => noop
            m_bufferPosIt = m_buffer.cend();
        }
        else
        {
            // new pos outside existing buffer => read chunk around new pos from stream
            updateChunk(pos);
        }
    }
}

std::ios::pos_type sciformats::jdx::BufferedTextReader::getLength()
{
    const std::ios::pos_type current = m_streamPtr->tellg();
    m_streamPtr->seekg(0, std::ios::end);
    const std::ios::pos_type length = m_streamPtr->tellg();
    m_streamPtr->seekg(current, std::ios::beg);
    return length;
}

bool sciformats::jdx::BufferedTextReader::eof() const
{
    if (std::distance(m_buffer.cbegin(), m_bufferPosIt) < m_buffer.size())
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
        throw ParseException(
            "End of file reached, but std::ios::eofbit not set.");
    }
    return false;
}

std::string sciformats::jdx::BufferedTextReader::readLine()
{    
    auto nextChunkStartPos = m_bufferBasePos + static_cast<std::ios::pos_type>(m_bufferMaxSize);
    if (m_bufferPosIt == m_buffer.cend() && nextChunkStartPos >= getLength())
    {
        throw ParseException("Error reading line from istream.");
    }
    auto posIt = std::find(m_bufferPosIt, m_buffer.cend(), '\n');    
    std::string out{m_bufferPosIt, posIt};
    // set new buffer position either past end or past found LF
    m_bufferPosIt = posIt == m_buffer.cend() ? m_buffer.cend() : ++posIt;
    while (posIt == m_buffer.cend())
    {
        // no LF encountered => load next chunk if available and continue search
        if (nextChunkStartPos >= getLength())
        {
            break;
        }
        updateChunk(nextChunkStartPos);
    }
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
