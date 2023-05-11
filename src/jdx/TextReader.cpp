#include "jdx/TextReader.hpp"
#include "jdx/ParseException.hpp"

#include <fstream>
#include <sstream>

sciformats::jdx::TextReader::TextReader(std::unique_ptr<std::istream> streamPtr)
    : m_streamPtr{std::move(streamPtr)}
{
    setStreamFlags();
}

sciformats::jdx::TextReader::TextReader(const std::string& filePath)
    // : m_streamPtr{}
    : m_streamPtr{std::make_unique<std::ifstream>(filePath)}
    // , m_bufferPtr{nullptr}
{
    // load fully into memory speeds up emscripten reading a lot
    // TODO: implement chunked loading
    // auto streamPtr = std::make_unique<std::ifstream>(filePath);
    // auto stringstreamPtr = std::make_unique<std::stringstream>();
    // *stringstreamPtr << streamPtr->rdbuf();
    // m_streamPtr = std::move(stringstreamPtr);

    // little to no effect
    // constexpr size_t bufferSize = 1024 * 1024;
    // auto bufferPtr = std::make_unique<char[]>(bufferSize);
    // auto streamPtr = std::make_unique<std::ifstream>();
    // streamPtr->rdbuf()->pubsetbuf(bufferPtr.get(), bufferSize);
    // streamPtr->open(filePath);
    // m_streamPtr = std::move(streamPtr);
    // m_bufferPtr = std::move(bufferPtr);
    setStreamFlags();
}

void sciformats::jdx::TextReader::setStreamFlags()
{
    if (m_streamPtr == nullptr)
    {
        throw ParseException("Text reader input stream is null.");
    }
    // the underlying getline() method sets eofbit at end of file, so do not set
    // std::ios::eofbit
    m_streamPtr->exceptions(std::ios::failbit | std::ios::badbit);
}

std::ios::pos_type sciformats::jdx::TextReader::tellg() const
{
    return m_streamPtr->tellg();
}

void sciformats::jdx::TextReader::seekg(
    std::ios::pos_type position, std::ios_base::seekdir seekdir)
{
    m_streamPtr->seekg(position, seekdir);
}

std::ios::pos_type sciformats::jdx::TextReader::getLength()
{
    const std::ios::pos_type current = m_streamPtr->tellg();
    m_streamPtr->seekg(0, std::ios::end);
    const std::ios::pos_type length = m_streamPtr->tellg();
    m_streamPtr->seekg(current, std::ios::beg);
    return length;
}

bool sciformats::jdx::TextReader::eof() const
{
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

std::string sciformats::jdx::TextReader::readLine()
{
    std::string out{};
    if (std::getline(*m_streamPtr, out))
    {
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
    throw ParseException("Error reading line from istream.");
}
