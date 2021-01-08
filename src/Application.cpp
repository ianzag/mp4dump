
#include "Application.h"
#include "StreamDownloader.h"

#include <iostream>
#include <string_view>

namespace mp4dump {

bool Application::parseCommandLine(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        const auto opt = std::string_view(argv[i]);
        // TODO: Parse options

        // URL may be defined only once
        if (m_url.empty()) {
            m_url = opt;
        } else {
            printUsage();
            return false;
        }
    }

    if (m_url.empty()) {
        printUsage();
        return false;
    }

    return true;
}

bool Application::run()
{
    std::cout << "Start to fetch data stream from '" << m_url << "'" << std::endl;

    // Download stream from remote site and parse it
    StreamDownloader streamDownloader(m_url);
    streamDownloader.downloadStream([](const DataView& buffer)
    {
        // XXX Parse received data stream
    });

    std::cout << "Finish to fetch data stream" << std::endl;
    return true;
}

void Application::printUsage()
{
    std::cout << "Usage: mp4dump [options] <url>" << std::endl;
}

} // namespace
