
#include "DownloaderFactory.h"
#include "FileDownloader.h"
#include "HttpDownloader.h"

#include <regex>
#include <sstream>

namespace mp4dump {

std::unique_ptr<StreamDownloader> DownloaderFactory::createDownloader(std::string url) const
{
    const std::regex regSchema("^([0-9a-zA-Z]+)://(.+)$");
    std::smatch matches;
    if (std::regex_match(url, matches, regSchema)) {
        const auto schema = matches[1];
        // TODO: Compare schema case insensitive
        if (schema == "http" || schema == "https") {
            return std::make_unique<HttpDownloader>(url);
        } else if (schema == "file") {
            return std::make_unique<FileDownloader>(matches[2]);
        } else {
            std::ostringstream os;
            os << "Invalid schema '" << schema << "'";
            throw std::runtime_error(os.str());
        }
    }

    // Treat URL without schema as local file path
    return std::make_unique<FileDownloader>(url);
}

} // namespace
