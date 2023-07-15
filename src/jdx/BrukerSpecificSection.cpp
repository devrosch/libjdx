#include "jdx/BrukerSpecificSection.hpp"
#include "jdx/ParseException.hpp"
#include "util/LdrUtils.hpp"

sciformats::jdx::BrukerSpecificSection::BrukerSpecificSection(
    io::TextReader& reader, std::optional<std::string>& nextLine)
{
    parse(reader, nextLine);
}

void sciformats::jdx::BrukerSpecificSection::parse(
    io::TextReader& reader, std::optional<std::string>& nextLine)
{
    if (!nextLine.has_value()
        || !util::isBrukerSpecificSectionStart(nextLine.value()))
    { }
    if (reader.eof())
    {
        throw ParseException(
            "Illegal body of Bruker specific section. No content in: "
            + nextLine.value_or(""));
    }

    m_title = util::stripLineComment(nextLine.value(), false, true)
                  .second.value_or("");
    nextLine = reader.readLine();
    if (!isDashedLine(nextLine))
    {
        throw ParseException(
            "Illegal body of Bruker specific section. No dash line after: "
            + m_title);
    }

    while (
        !reader.eof() && !util::isBrukerSpecificSectionStart(nextLine.value()))
    {
        if (isSectionEnd(nextLine))
        {
            nextLine = reader.readLine();
            if (!isDashedLine(nextLine))
            {
                throw ParseException("Illegal end of Bruker specific "
                                     "section. No dash line after: "
                                     + std::string{s_sectionEndText});
            }
            nextLine = reader.eof() ? std::optional<std::string>{}
                                    : reader.readLine();
            break;
        }
        // TODO: implement actual parsing, not just skipping lines
        nextLine = reader.readLine();
    }
}

bool sciformats::jdx::BrukerSpecificSection::isSectionEnd(
    std::optional<std::string>& nextLine)
{
    return nextLine.has_value()
           && nextLine.value().rfind(s_sectionEndText, 0) == 0;
}

bool sciformats::jdx::BrukerSpecificSection::isDashedLine(
    std::optional<std::string>& nextLine)
{
    if (!nextLine.has_value())
    {
        return false;
    }
    auto [content, comment]
        = util::stripLineComment(nextLine.value(), true, true);
    if (content.empty() && comment.has_value())
    {
        return comment.value().rfind("-----", 0) == 0;
    }
    return false;
}

std::string sciformats::jdx::BrukerSpecificSection::getContent() const
{
    return m_content;
}
