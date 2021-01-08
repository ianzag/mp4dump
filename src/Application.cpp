
#include "Application.h"

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
    // XXX Run application

    return true;
}

void Application::printUsage()
{
    std::cout << "Usage: mp4dump [options] <url>" << std::endl;
}

} // namespace
