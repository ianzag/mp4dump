/**
 * This is a simple utility that get input MP4 file, parses it and prints discovered file structure.
 * File can be read from local file system or from remote HTTP server using GET request.
 * This dumper is more like a skeleton and does not handle all/much of box types.
 * Technically it was not designed as an application for real usage but just as a code test.
 * It is free and distributed under MIT license. Do with it whatever you want to.
 */

#include "Application.h"

#include <cstddef>
#include <iostream>
#include <exception>

int main(int argc, char* argv[])
{
    try {
        mp4dump::Application app;
        if (app.parseCommandLine(argc, argv) && app.run()) {
            return EXIT_SUCCESS;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Error: Unhandled exception" << std::endl;
    }
    return EXIT_FAILURE;
}
