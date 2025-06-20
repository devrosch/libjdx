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
#ifndef JDX_DATALDR_HPP
#define JDX_DATALDR_HPP

#include "io/TextReader.hpp"
#include "jdx/Ldr.hpp"

#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace libjdx::jdx
{
/**
 * @brief Base class for JCAMP-DX data records.
 */
class DataLdr : public Ldr
{
public:
    /**
     * @brief The record's variable list.
     * @return The record's variable list.
     */
    [[nodiscard]] const std::string& getVariableList() const;

protected:
    DataLdr(
        std::string label, std::string variableList, io::TextReader& reader);

    [[nodiscard]] io::TextReader& getReader() const;
    /**
     * @brief Validates if input is a valid data LDR.
     * @param label LDR label.
     * @param variableList First line LDR value. Must represent a variable list.
     * @param expectedLabel The expected LDR label.
     * @param expectedVariableList The expected variable list.
     * @throws If label or variable list don't match expectations.
     */
    static void validateInput(const std::string& label,
        const std::string& variableList, const std::string& expectedLabel,
        const std::vector<std::string>& expectedVariableLists);

    template<typename R>
    R callAndResetStreamPos(const std::function<R()>& func) const;

private:
    const std::string m_variableList;
    io::TextReader& m_reader;
    std::streampos m_dataPos;
};

template<typename R>
R libjdx::jdx::DataLdr::callAndResetStreamPos(
    const std::function<R()>& func) const
{
    auto pos = m_reader.eof() ? std::nullopt
                              : std::optional<std::streampos>(m_reader.tellg());
    auto resetPosition = [pos, this] {
        if (pos)
        {
            m_reader.seekg(pos.value());
        }
        else
        {
            m_reader.seekg(0, std::ios_base::end);
        }
    };

    try
    {
        m_reader.seekg(m_dataPos);
        R returnValue = func();
        resetPosition();
        return returnValue;
    }
    catch (...)
    {
        try
        {
            resetPosition();
        }
        catch (...)
        {
        }
        throw;
    }
}

} // namespace libjdx::jdx

#endif // JDX_DATALDR_HPP
