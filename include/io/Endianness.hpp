#ifndef IO_ENDIANNESS_HPP
#define IO_ENDIANNESS_HPP

#include <cstdint> // for uint8_t

namespace sciformats::io
{
/**
 * @brief The endianness indicates the byte order of data.
 */
enum class Endianness : uint8_t
{
    LittleEndian, ///< The least significant byte comes first.
    BigEndian, ///< The most significant byte comes first.
};

} // namespace sciformats::io

#endif // IO_ENDIANNESS_HPP
