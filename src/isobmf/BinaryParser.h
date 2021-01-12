
#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>

namespace isobmf {

template <std::size_t MaxSize>
class BinaryParser
{
public:
    std::size_t size() const noexcept
    {
        return m_pos;
    }

    std::size_t capacity() const noexcept
    {
        return m_buf.size();
    }

    bool empty() const noexcept
    {
        return size() == 0;
    }

    void reset() noexcept
    {
        m_pos = 0;
    }

    std::size_t putChar(std::uint8_t ch)
    {
        if (m_pos >= m_buf.size()) {
            throw std::out_of_range("Binary parser overflow");
        }
        m_buf[m_pos++] = ch;
        return m_pos;
    }

    template <class T>
    T getAs() const
    {
        if (m_pos < sizeof(T)) {
            throw std::out_of_range("Binary parser underflow");
        }
        T result = 0;
        std::size_t shift = (sizeof(T) - 1) * 8;
        for (std::size_t i = 0; i < sizeof(T); i++) {
            result |= static_cast<T>(m_buf[i]) << shift;
            shift -= 8;
        }
        return result;
    }

    const auto begin() const noexcept
    {
        return m_buf.begin();
    }

    const auto end() const noexcept
    {
        return m_buf.begin() + m_pos;
    }

private:
    std::array<std::uint8_t, MaxSize> m_buf;
    std::size_t m_pos = 0;
};

} // namespace
