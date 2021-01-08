
#pragma once

#include <string>

namespace mp4dump {

/**
 * @brief Main application class
 */
class Application
{
public:
    /**
     * @brief Parse command line options
     * @return True on success or false on failure
     */
    bool parseCommandLine(int argc, char* argv[]);

    /**
     * @brief Run application
     * @return True on success or false on failure
     */
    bool run();

private:
    void printUsage();

    std::string m_url; ///< URL to fetch remote data stream
};

} // namespace
