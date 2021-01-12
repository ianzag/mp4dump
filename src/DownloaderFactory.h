
#pragma once

#include "StreamDownloader.h"

#include <memory>
#include <string>

namespace mp4dump {

class DownloaderFactory
{
public:
    /**
     * @brief Create stream downloader for given URL
     * @param url Target URL
     * @return Downloader instance
     * @throw std::exception on failure
     */
    std::unique_ptr<StreamDownloader> createDownloader(std::string url) const;
};

} // namespace
