
#pragma once

#include <cstdint>
#include <functional>
#include <string_view>

namespace mp4dump {

/**
 * @brief Interface for data stream downloader
 */
class StreamDownloader
{
public:
    virtual ~StreamDownloader() = default;

    using DataView = std::basic_string_view<std::uint8_t>;
    using DataCallback = std::function<void(const DataView& data)>;

    /**
     * @brief Download stream and feed it into supplied callback
     * @param dataCallback Target callback to feed
     */
    virtual void downloadStream(DataCallback dataCallback) = 0;
};

} // namespace
