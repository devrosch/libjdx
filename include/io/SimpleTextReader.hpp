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
#ifndef IO_SIMPLETEXTREADER_HPP
#define IO_SIMPLETEXTREADER_HPP

#include <istream>
#include <memory>

namespace libjdx::io
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
} // namespace libjdx::io

#endif
