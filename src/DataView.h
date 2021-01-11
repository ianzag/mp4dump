
#pragma once

#include <cstddef>
#include <cstdint>

namespace mp4dump {

/**
 * @brief Tiny wrapper to access fixed constant data buffer
 */
class DataView
{
public:
    DataView() noexcept = default;

    DataView(const void* data, std::size_t size) noexcept
        : m_data{data}, m_size{size} {}

    std::size_t size() const noexcept
    {
        return m_size;
    }

    bool empty() const noexcept
    {
        return m_size == 0 || !m_data;
    }

    const std::uint8_t* data() const noexcept
    {
        return static_cast<const std::uint8_t*>(m_data);
    }

    const std::uint8_t* begin() const noexcept
    {
        return data();
    }

    const std::uint8_t* end() const noexcept
    {
        return data() + size();
    }

    std::uint8_t operator[](std::size_t pos) const noexcept
    {
        return data()[pos];
    }

private:
    const void* m_data = nullptr;
    std::size_t m_size = 0;
};

} // namespace
