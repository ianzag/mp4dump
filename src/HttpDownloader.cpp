
#include "HttpDownloader.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <sstream>

namespace mp4dump {

HttpDownloader::HttpDownloader(const std::string& url)
    : m_url{url}
    , m_curl{curl_easy_init()}
{
    if (!m_curl) {
        throw std::runtime_error("Failed to download stream (Cannot init CURL context)");
    }
}

HttpDownloader::~HttpDownloader()
{
    if (m_curl) {
        curl_easy_cleanup(m_curl);
    }
}

void HttpDownloader::downloadStream(DataCallback dataCallback)
{
    setOption(CURLOPT_URL, m_url);
    setOption(CURLOPT_HTTPGET, 1L);
    setOption(CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
    setOption(CURLOPT_FOLLOWLOCATION, 1L);
    setOption(CURLOPT_MAXREDIRS, 5); // TODO: Make it configureable
    setOption(CURLOPT_FAILONERROR, 1L);
    setOption(CURLOPT_ERRORBUFFER, m_errorBuffer.data());
    setOption(CURLOPT_WRITEDATA, this);
    setOption(CURLOPT_WRITEFUNCTION, reinterpret_cast<void *>(cbWriteFunction));

    m_dataCallback = dataCallback;
    m_errorBuffer[0] = '\0';

    const auto error = curl_easy_perform(m_curl);
    if (error != CURLE_OK) {
        std::ostringstream os;
        if (const auto errorMessage = getErrorMessage()) {
            os << "Failed to perform CURL request (" << errorMessage << ")";
        } else {
            os << "Failed to perform CURL request (Error " << error << ")";
        }
        throw std::runtime_error(os.str());
    }
}

bool HttpDownloader::handleWrite(const DataView& data)
{
    // Note: Handler may be executed in context of other thread!
    try {
        if (m_dataCallback) {
            m_dataCallback(data);
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to handle CURL reply (" << e.what() << ")" << std::endl;
    } catch (...) {
        std::cerr << "Error: Failed to handle CURL reply (Unknown exception)" << std::endl;
    }
    return false;
}

std::size_t HttpDownloader::cbWriteFunction(char* ptr, std::size_t size, std::size_t nmemb, void* data)
{
    if (ptr && data) {
        const auto totalSize = nmemb * size;
        if (static_cast<HttpDownloader*>(data)->handleWrite(DataView(reinterpret_cast<std::uint8_t*>(ptr), totalSize))) {
            return totalSize;
        }
    }
    return 0;
}

void HttpDownloader::setOption(CURLoption option, long value)
{
    const auto error = curl_easy_setopt(m_curl, option, value);
    if (error != CURLE_OK) {
        std::ostringstream os;
        os << "Failed to set CURL option " << option << " (Error " << error << ")";
        throw std::runtime_error(os.str());
    }
}

void HttpDownloader::setOption(CURLoption option, void* value)
{
    const auto error = curl_easy_setopt(m_curl, option, value);
    if (error != CURLE_OK) {
        std::ostringstream os;
        os << "Failed to set CURL option " << option << " (Error " << error << ")";
        throw std::runtime_error(os.str());
    }
}

void HttpDownloader::setOption(CURLoption option, const std::string& value)
{
    // As CURL docs says:
    // ===========
    // Before version 7.17.0, strings were not copied. Instead the user was forced
    // keep them available until libcurl no longer needed them.
    // ===========
    // To be compatible with any version of CURL keep a local copy of string option.
    //
    // TODO: Check CURL version and make a local copy only for old ones?
    //
    const auto p = m_stringOptions.emplace(option, value);
    const auto it = p.first;
    if (!p.second) {
        it->second = value;
    }

    const auto error = curl_easy_setopt(m_curl, option, it->second.c_str());
    if (error != CURLE_OK) {
        std::ostringstream os;
        os << "Failed to set CURL option " << option << " (Error " << error << ")";
        throw std::runtime_error(os.str());
    }
}

const char* HttpDownloader::getErrorMessage() const
{
    // Search for non-empty zero terminated string
    const auto it = std::find_if(m_errorBuffer.begin(), m_errorBuffer.end(), [](char ch){ return ch == '\0';});
    if (it != m_errorBuffer.end() && it != m_errorBuffer.begin()) return m_errorBuffer.data();
    return nullptr;
}

} // namespace
