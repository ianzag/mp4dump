
#pragma once

#include <cctype>
#include <algorithm>
#include <string>
#include <string_view>

namespace utils {

/**
 * @brief Convert string to lower case
 * @param s Input string to convert
 * @return Converted string
 * @note Should be fine for ASCII. For other charsets please review!
 */
std::string toLowerCase(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char ch)
    {
        return std::tolower(ch);
    });
    return s;
}

std::string toLowerCase(const std::string_view& s)
{
    std::string result(s.begin(), s.end());
    return toLowerCase(result);
}

} // namesapce
