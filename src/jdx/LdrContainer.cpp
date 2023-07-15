#include "jdx/LdrContainer.hpp"
#include "util/LdrUtils.hpp"
#include "util/StringUtils.hpp"

std::optional<const std::string>
sciformats::jdx::LdrContainer::parseStringValue(
    std::string& value, io::TextReader& reader)
{
    util::trim(value); // trim first line value only
    while (!reader.eof())
    {
        const auto line = reader.readLine();
        if (util::isLdrStart(line))
        {
            return line;
        }
        auto [content, comment] = util::stripLineComment(line);
        if (!content.empty() && !value.empty() && value.back() == '=')
        {
            // account for terminal "=" as non line breaking marker
            value.pop_back();
            value.append(line);
        }
        else if (content.empty() && comment.has_value())
        {
            if (util::isBrukerSpecificSectionStart(line))
            {
                return line;
            }
        }
        else
        {
            value.append('\n' + line);
        }
    }
    return std::nullopt;
}
