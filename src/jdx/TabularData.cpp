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
#include "jdx/TabularData.hpp"
#include "util/LdrUtils.hpp"
#include "util/PeakAssignmentsParser.hpp"
#include "util/StringUtils.hpp"

libjdx::jdx::TabularData::TabularData(
    std::string label, std::string variableList, io::TextReader& reader)
    : DataLdr(std::move(label), std::move(variableList), reader)
{
}

std::optional<std::string> libjdx::jdx::TabularData::getWidthFunction()
{
    auto appendToDescription
        = [](std::string comment, std::string& description) {
              if (!description.empty())
              {
                  description += '\n';
              }
              util::trim(comment);
              description.append(comment);
          };

    auto getCommentLines = [this, &appendToDescription]() {
        // comment $$ in line(s) following LDR start may contain peak function
        auto& reader = getReader();
        auto readerPos = reader.tellg();
        std::string line{};
        std::string functionDescription{};
        while (!reader.eof() && !util::isLdrStart(line = reader.readLine())
               && util::isPureComment(line))
        {
            readerPos = reader.tellg();
            auto comment = util::stripLineComment(line).second;
            appendToDescription(comment.value(), functionDescription);
        }
        // reset reader position to start of first assignment or start of next
        // LDR
        reader.seekg(readerPos);
        // return
        return functionDescription.empty()
                   ? std::nullopt
                   : std::optional<std::string>{functionDescription};
    };

    return callAndResetStreamPos<std::optional<std::string>>(getCommentLines);
}
