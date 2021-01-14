
#include "Application.h"
#include "DownloaderFactory.h"
#include "isobmf/BaseParserFactory.h"
#include "isobmf/ContainerBoxParser.h"

#include <iostream>
#include <string_view>

namespace mp4dump {

/**
 * @brief Treat data stream as a box container
 */
class StreamParser final : public isobmf::ContainerBoxParser
{
public:
    explicit StreamParser(const isobmf::ParserFactory& m_boxFactory)
        : ContainerBoxParser(m_boxFactory) {}

    void startParse() override
    {
        std::cout << "Stream {" << std::endl;
    }

    void endParse() override
    {
        std::cout << "}" << std::endl;
    }
};

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

    // Create stream downloader instance
    DownloaderFactory downloaderFactory;
    auto streamDownloader = downloaderFactory.createDownloader(m_url);

    // Create stream parser
    // TODO: Detect which parser factory to use from file type (MP4, 3GP etc)
    isobmf::BaseParserFactory parserFactory;
    StreamParser streamParser(parserFactory);

    // Download stream, parse and print it
    streamParser.startParse();
    streamDownloader->downloadStream([&](const auto& buffer)
    {
        // Feed pieces of stream to the parser
        for (const auto ch : buffer) {
            streamParser.parseChar(ch);
        }
    });
    streamParser.endParse();

    std::cout << "Finish to fetch data stream" << std::endl;
    return true;
}

void Application::printUsage()
{
    std::cout << "Usage: mp4dump [options] <url>" << std::endl;
}

} // namespace
