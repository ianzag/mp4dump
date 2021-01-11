
#pragma once

#include "IsoBmfInterfaces.h"

#include <array>
#include <iostream>
#include <stdexcept>
#include <string>

namespace isobmf {

/**
 * @brief Base class for all box parsers
 *
 * Handle common box properties like name, size, parent etc.
*/
class BaseBoxParser : public BoxParser
{
public:
    BaseBoxParser() = default;

    BaseBoxParser(std::string boxName, BoxSize boxSize, const BoxParser* parentBox)
        : m_boxName{std::move(boxName)}
        , m_boxSize{boxSize}
        , m_parentBox{parentBox}
    {}

    const std::string& getName() const noexcept override
    {
        return m_boxName;
    }

    BoxSize getSize() const noexcept override
    {
        return m_boxSize;
    }

    unsigned int getLevel() const noexcept override
    {
        return m_parentBox ? m_parentBox->getLevel() + 1 : 0;
    }

    /**
     * @brief Print indent string depending on box position inside box tree
     * @param os Output stream
     * @return Output stream
     */
    std::ostream& printIndent(std::ostream& os) const
    {
        auto level = getLevel();
        for (; level > 0; --level) {
            os << "  ";
        }
        return os;
    }

protected:
    // Temporary parse buffer
    std::array<std::uint8_t, 64> m_buf;
    std::size_t m_pos = 0;

    /**
     * @brief Append character to buffer and return new buffer length
     * @param ch Char to append
     * @return Buffer length
     */
    std::size_t putChar(std::uint8_t ch)
    {
        if (m_pos >= m_buf.size()) {
            throw std::out_of_range("Box parser buffer overflow");
        }
        m_buf[m_pos++] = ch;
        return m_pos;
    }

    template <class T>
    T getAs() const noexcept
    {
        T result = 0;
        std::size_t shift = (sizeof(T) - 1) * 8;
        for (std::size_t i = 0; i < sizeof(T); i++) {
            result |= static_cast<T>(m_buf[i]) << shift;
            shift -= 8;
        }
        return result;
    }

private:
    std::string m_boxName;
    BoxSize m_boxSize = 0;
    const BoxParser* m_parentBox = nullptr;
};

} // namespace
