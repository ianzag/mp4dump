
#pragma once

#include <cstddef>
#include <array>
#include <regex>

namespace isobmf {

enum class FileFormat
{
    InProgress, ///< Detection is incompleted
    Unknown,    ///< Failed to detect file format
    XML,        ///< XML detected
};

/**
 * File format detector. It attempts to guess file format from first N file characters.
 * It's very simple. Actually it's soo simple that all what it does is detects an XML document preamble.
 */
template <std::size_t MaxSize>
class FileFormatDetector
{
public:
    FileFormat putChar(char ch)
    {
        m_buf[m_pos++] = ch;
        if (ch == '>') { // End of XML preamble
            const std::regex regXml("^<\\?xml.+\\?>$");
            if (std::regex_match(m_buf.begin(), m_buf.begin() + m_pos, regXml)) {
                return FileFormat::XML;
            }
        }
        return m_pos < m_buf.size() ? FileFormat::InProgress : FileFormat::Unknown;
    }

    void reset() noexcept
    {
        m_pos = 0;
    }

private:
    std::array<char, MaxSize> m_buf;
    std::size_t m_pos = 0;
};


} // namespace
