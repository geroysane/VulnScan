#include "CliApplication.h"
// #include "core/common/Logger.h"  // TODO: Fix Logger blocking issue
#include <iostream>

/**
 * @brief Main entry point for VulnScan CLI application
 */
int main(int argc, char *argv[]) {
    // TODO: Fix Logger - currently blocking on initialize()
    // Logger::initialize("vulnscan_cli.log", true);
    // Logger::info("VulnScan CLI starting...");

    // Create and initialize CLI application
    CliApplication app;
    if (!app.initialize(argc, argv)) {
        std::cerr << "Failed to initialize CLI application" << std::endl;
        return 1;
    }

    // Run application
    int exitCode = app.run();

    // Logger::info("VulnScan CLI completed");
    // Logger::shutdown();

    return exitCode;
}
