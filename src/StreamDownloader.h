
#pragma once

#include "DataView.h"

#include <array>
#include <functional>
#include <map>
#include <string>
#include <curl/curl.h>

namespace mp4dump {

/**
 * @brief Simple downloader to fetch data stream from remote site with HTTP protocol
 */
class StreamDownloader
{
public:
    // Non-copyable
    StreamDownloader(const StreamDownloader&) = delete;
    StreamDownloader& operator = (const StreamDownloader&) = delete;

    StreamDownloader(const std::string& url);
    ~StreamDownloader();

    using DataCallback = std::function<void(const DataView& data)>;

    void downloadStream(DataCallback dataCallback);

private:
    bool handleWrite(const DataView& data);
    static std::size_t cbWriteFunction(char* ptr, std::size_t size, std::size_t nmemb, void* data);

    void setOption(CURLoption option, long value);
    void setOption(CURLoption option, void* value);
    void setOption(CURLoption option, const std::string& value);
    const char* getErrorMessage() const;

    std::string m_url;
    CURL* m_curl = nullptr;
    std::map<CURLoption, std::string> m_stringOptions;
    DataCallback m_dataCallback;
    std::array<char, CURL_ERROR_SIZE + 1> m_errorBuffer;
};

} // namespace
