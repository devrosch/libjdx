#ifndef JDX_BRUKERRELAXSECTION_HPP
#define JDX_BRUKERRELAXSECTION_HPP

#include "io/TextReader.hpp"

#include <optional>
#include <string>
#include <vector>

namespace sciformats::jdx
{
/**
 * @brief A JCAMP-DX Bruker specific RELAX section.
 * @details This section starts with:
 *
 * ##$RELAX=
 * ##$BRUKER FILE EXP= <file name>
 *
 * and ends with when another LDR starts (which could be another RELAX section)
 * or a Bruker specific parameters section starts indicated by:
 *
 * $$ Bruker specific parameters <optional additional text>
 * $$ --------------------------
 *
 */
class BrukerRelaxSection
{
public:
    /**
     * @brief Constructs BrukerRelaxSection.
     * @param label The label of the first line of the record, i.e.
     * "RELAX".
     * @param value The value of the first line of the record, i.e. blank.
     * @param reader Text reader with JCAMP-DX data. The reader position is
     * assumed to be at the start of the second line of the record. The reader
     * is expected to exist for the lifetime of this object.
     * @param nextLine The first line of the record, i.e., the one containing
     * the section start text. Will contain the line following the record or
     * nullopt if the end of the reader has been reached.
     */
    BrukerRelaxSection(const std::string& label, const std::string& value,
        io::TextReader& reader, std::optional<std::string>& nextLine);

    /**
     * @brief Provides the name of the section.
     * @return The section name.
     */
    [[nodiscard]] std::string getName() const;

    /**
     * @brief Provides the contents of the section.
     * @return The section content.
     */
    [[nodiscard]] std::string getContent() const;

private:
    static constexpr const char* s_label = "$RELAX";
    static constexpr const char* s_labelFileName = "$BRUKERFILEEXP";

    static void validate(const std::string& label, std::string value,
        std::optional<std::string>& nextLine);
    void parse(io::TextReader& reader, std::optional<std::string>& nextLine);

    std::string m_name;
    std::string m_content;
};
} // namespace sciformats::jdx

#endif
