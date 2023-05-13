#ifndef JDX_BUFFEREDTEXTREADER_HPP
#define JDX_BUFFEREDTEXTREADER_HPP

#include <istream>
#include <memory>
#include <vector>

namespace sciformats::jdx
{
/**
 * @brief Provides mechanisms to read textual data partially buffered in memory.
 */
class BufferedTextReader
{
public:
    /**
     * @brief Constructs from stream.
     * @param streamPtr An open input stream.
     * @param bufferSize The size of the buffer in bytes.
     */
    explicit BufferedTextReader(std::unique_ptr<std::istream> streamPtr, size_t bufferSize = s_bufferDefaultMaxSize);

    /**
     * @brief Constructs from file.
     * @param filePath Path to the file.
     * @param bufferSize The size of the buffer in bytes.
     */
    explicit BufferedTextReader(const std::string& filePath, size_t bufferSize = s_bufferDefaultMaxSize);

    /**
     * @brief tellg Get the current read position in the data.
     * @return The current read position.
     */
    [[nodiscard]] std::ios::pos_type tellg() const;

    /**
     * @brief seekg Set the read position in the data.
     */
    void seekg(std::ios::off_type, std::ios_base::seekdir seekdir = std::ios_base::beg);

    /**
     * @brief getLength The length (in chars) of the input data.
     * @return Total length (in chars) of the input data.
     */
    std::ios::pos_type getLength();

    /**
     * @brief eof End of file reached?
     * @return True if end of file reached, false otherwise.
     *
     * @note The behavior of this method deviates from isteram.eof(). This
     * method returns true when the EOF has been reached without the need for a
     * previous failing read operation.
     */
    [[nodiscard]] bool eof() const;

    /**
     * @brief readLine Reads one line of text terminated by \\r\\n or \\n.
     * @return The line read without trailing \\n or \\r\\n.
     */
    std::string readLine();

private:
    static constexpr size_t s_bufferDefaultMaxSize = 4 * 1024;
    std::unique_ptr<std::istream> m_streamPtr;
    size_t m_bufferMaxSize;
    std::vector<char> m_buffer;
    std::ios::pos_type m_bufferBasePos;
    std::vector<char>::const_iterator m_bufferPosIt;

    void setStreamFlags();
    std::ios::pos_type calculateAbsolutePosition(std::ios::pos_type position, std::ios_base::seekdir seekdir);
    void updateBuffer(std::ios::pos_type position);
};
} // namespace sciformats::jdx

#endif
