
#pragma once

#include "IsoBmfTypes.h"

#include <string>

namespace isobmf {

/**
 * @brief Construct box type opcode from its short name e.g. 'moov'
 * @param name Box type name
 * @return Box type opcode
 */
inline constexpr BoxType makeBoxType(const char name[4])
{
    return (static_cast<BoxType>(name[0]) << 24) |
           (static_cast<BoxType>(name[1]) << 16) |
           (static_cast<BoxType>(name[2]) <<  8) |
           (static_cast<BoxType>(name[3])      );
}

/**
 * @brief Construct short box type name from box type opcode
 * @param boxType Box type opcode
 * @return Box type name
 */
inline std::string makeBoxName(BoxType boxType)
{
    std::string result;
    result += (boxType >> 24) & 0xFF;
    result += (boxType >> 16) & 0xFF;
    result += (boxType >>  8) & 0xFF;
    result += (boxType >>  0) & 0xFF;
    return result;
}

inline std::string makeBrandName(std::uint32_t brandCode)
{
    std::string result;
    result += (brandCode >> 24) & 0xFF;
    result += (brandCode >> 16) & 0xFF;
    result += (brandCode >>  8) & 0xFF;
    result += (brandCode >>  0) & 0xFF;
    return result;
}

} // namespace
