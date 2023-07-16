#include "jdx/BrukerSpecificParameters.hpp"
#include "jdx/ParseException.hpp"
#include "util/LdrUtils.hpp"

sciformats::jdx::BrukerSpecificParameters::BrukerSpecificParameters(
    io::TextReader& reader, std::optional<std::string>& nextLine)
{
    parse(reader, nextLine);
}

void sciformats::jdx::BrukerSpecificParameters::parse(
    io::TextReader& reader, std::optional<std::string>& nextLine)
{
    if (!nextLine.has_value()
        || !util::isBrukerSpecificSectionStart(nextLine.value()))
    {
        throw ParseException("Illegal start of Bruker specific section: "
                             + nextLine.value_or(""));
    }
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
    nextLine = reader.eof() ? std::optional<std::string>{} : reader.readLine();

    parseLdrs(reader, nextLine);
}

bool sciformats::jdx::BrukerSpecificParameters::isDashedLine(
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

void sciformats::jdx::BrukerSpecificParameters::parseLdrs(
    io::TextReader& reader, std::optional<std::string>& nextLine)
{
    while (nextLine.has_value()
           && !util::isBrukerSpecificSectionStart(nextLine.value())
           && !util::isBrukerSpecificSectionEnd(nextLine.value()))
    {
        // TODO: skip other leading comments?
        auto [label, value] = util::parseLdrStart(nextLine.value());
        nextLine = parseStringValue(value, reader);
        m_content.emplace_back(label, value);
    }

    if (!nextLine.has_value()
        || (!util::isBrukerSpecificSectionStart(nextLine.value())
            && !util::isBrukerSpecificSectionEnd(nextLine.value())))
    {
        throw ParseException("Unexpected end of Bruker specific section:"
                             + std::string{s_sectionEndText});
    }

    if (util::isBrukerSpecificSectionEnd(nextLine.value()))
    {
        // skip dashes after section end marker
        nextLine
            = reader.eof() ? std::optional<std::string>{} : reader.readLine();
        if (!isDashedLine(nextLine))
        {
            throw ParseException("Illegal end of Bruker specific "
                                 "section. No dash line after: "
                                 + std::string{s_sectionEndText});
        }
        nextLine
            = reader.eof() ? std::optional<std::string>{} : reader.readLine();
    }
}

std::vector<sciformats::jdx::StringLdr>
sciformats::jdx::BrukerSpecificParameters::getContent() const
{
    return m_content;
}
