
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace utils {

std::string Base64Decode(const void* data, const std::size_t len);

inline auto Base64Decode(const std::string& s)
{
    return Base64Decode(s.data(), s.size());
}

inline auto Base64Decode(const std::string_view& s)
{
    return Base64Decode(s.data(), s.size());
}

} // namespace
