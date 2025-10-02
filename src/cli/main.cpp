#include "CliApplication.h"
#include "core/common/Logger.h"
#include <iostream>

/**
 * @brief Main entry point for VulnScan CLI application
 */
int main(int argc, char *argv[]) {
    // Initialize logger
    Logger::initialize("vulnscan_cli.log", true);
    Logger::info("VulnScan CLI starting...");

    // Create and initialize CLI application
    CliApplication app;
    if (!app.initialize(argc, argv)) {
        Logger::error("Failed to initialize CLI application");
        Logger::shutdown();
        return 1;
    }

    // Run application
    int exitCode = app.run();

    Logger::info("VulnScan CLI completed");
    Logger::shutdown();

    return exitCode;
}
