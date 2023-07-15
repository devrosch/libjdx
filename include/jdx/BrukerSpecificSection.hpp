#ifndef JDX_BRUKERSPECIFICSECTION_HPP
#define JDX_BRUKERSPECIFICSECTION_HPP

#include "io/TextReader.hpp"

#include <optional>
#include <string>
#include <vector>

namespace sciformats::jdx
{
/**
 * @brief A JCAMP-DX Bruker specific section.
 * @details This section starts with:
 *
 * $$ Bruker specific parameters <optional additional text>
 * $$ --------------------------
 *
 * and ends when another Bruker specific section starts or the section end is
 * indicated by:
 *
 * $$ End of Bruker specific parameters
 * $$ ---------------------------------
 */
class BrukerSpecificSection
{
public:
    /**
     * @brief Constructs BrukerSpecificSection.
     * @param reader Text reader with JCAMP-DX data. The reader position is
     * assumed to be at the start of the second line of the record. The reader
     * is expected to exist for the lifetime of this object.
     * @param nextLine The first line of the record, i.e., the one containing
     * the section start text. Will contain the line following the record or
     * nullopt if the end of the reader has been reached.
     */
    BrukerSpecificSection(
        io::TextReader& reader, std::optional<std::string>& nextLine);

    /**
     * @brief Provides the contents of the section as text. Lines are separated
     * by "\n".
     * @return The section content.
     */
    [[nodiscard]] std::string getContent() const;

private:
    // static constexpr const char* s_sectionStartText = "$$ Bruker specific
    // parameters";
    static constexpr const char* s_sectionEndText
        = "$$ End of Bruker specific parameters";

    void parse(io::TextReader& reader, std::optional<std::string>& nextLine);
    static bool isSectionEnd(std::optional<std::string>& nextLine);
    static bool isDashedLine(std::optional<std::string>& nextLine);

    std::string m_title;
    std::string m_content;
};
} // namespace sciformats::jdx

#endif // JDX_BRUKERSPECIFICSECTION_HPP
