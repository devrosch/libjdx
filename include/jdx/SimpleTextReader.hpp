#ifndef JDX_SIMPLETEXTREADER_HPP
#define JDX_SIMPLETEXTREADER_HPP

#include <istream>
#include <memory>

namespace sciformats::jdx
{
/**
 * @brief Provides mechanisms to read textual data.
 * Shallow wrapper over istream.
 */
class SimpleTextReader
{
public:
    /**
     * @brief Constructs from stream.
     * @param streamPtr An open input stream.
     */
    explicit SimpleTextReader(std::unique_ptr<std::istream> streamPtr);

    /**
     * @brief Constructs from file.
     * @param filePath Path to the file.
     */
    explicit SimpleTextReader(const std::string& filePath);

    /**
     * @brief Get the current read position in the data.
     * @return The current read position.
     */
    [[nodiscard]] std::ios::pos_type tellg() const;

    /**
     * @brief Set the read position in the data.
     */
    void seekg(std::ios::pos_type, std::ios_base::seekdir = std::ios_base::beg);

    /**
     * @brief The length (in chars) of the input data.
     * @return Total length (in chars) of the input data.
     */
    std::ios::pos_type getLength();

    /**
     * @brief End of file reached?
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
    std::unique_ptr<std::istream> m_streamPtr;

    void setStreamFlags();
};
} // namespace sciformats::jdx

#endif
