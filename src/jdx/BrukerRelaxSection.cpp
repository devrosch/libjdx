#include "jdx/BrukerRelaxSection.hpp"
#include "jdx/ParseException.hpp"
#include "util/LdrUtils.hpp"
#include "util/StringUtils.hpp"

sciformats::jdx::BrukerRelaxSection::BrukerRelaxSection(
    const std::string& label, const std::string& value, io::TextReader& reader,
    std::optional<std::string>& nextLine)
{
    validate(label, value, nextLine);
    parse(reader, nextLine);
}

void sciformats::jdx::BrukerRelaxSection::validate(const std::string& label,
    std::string value, std::optional<std::string>& nextLine)
{
    util::trim(value);
    if (label != s_label || !value.empty())
    {
        throw ParseException("Illegal start of Bruker " + std::string{s_label}
                             + " section: " + std::string{"##"} + label + ": "
                             + value);
    }
}

void sciformats::jdx::BrukerRelaxSection::parse(
    io::TextReader& reader, std::optional<std::string>& nextLine)
{
    if (reader.eof())
    {
        throw ParseException(
            "Premature end of Bruker " + std::string{s_label} + " section.");
    }
    nextLine = reader.readLine();
    if (util::isBrukerSpecificSectionStart(nextLine.value()))
    {
        // RELAX LDR immediately followed by
        // $$ Bruker specific parameters
        // => skip further parsing
        return;
    }
    auto [label, fileName] = util::parseLdrStart(nextLine.value());
    if (label.rfind(s_labelFileNameStart, 0) != 0)
    {
        throw ParseException("Illegal start of Bruker " + std::string{s_label}
                             + " section. " + std::string{s_label}
                             + " line not followed by "
                             + std::string{s_labelFileNameStart} + "....");
    }
    util::trim(fileName);
    m_name = fileName;

    while (!reader.eof())
    {
        nextLine = reader.readLine();
        if (!nextLine.has_value() || util::isLdrStart(nextLine.value())
            || util::isBrukerSpecificSectionStart(nextLine.value()))
        {
            break;
        }

        auto line = nextLine.value();
        if (line.rfind("$$ ", 0) == 0)
        {
            line = line.substr(3);
        }
        else if (line.rfind("$$", 0) == 0)
        {
            line = line.substr(2);
        }
        m_content.append(line);
        m_content.append("\n");
    }
}

std::string sciformats::jdx::BrukerRelaxSection::getName() const
{
    return m_name;
}

std::string sciformats::jdx::BrukerRelaxSection::getContent() const
{
    return m_content;
}
