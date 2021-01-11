
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
