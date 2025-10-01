#include <QCoreApplication>
#include <QTextStream>
#include "CommandLineParser.h"
#include "core/common/Logger.h"
#include "core/config/Configuration.h"

/**
 * @brief Main entry point for VulnScan CLI application
 */
int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // Set application information
    QCoreApplication::setOrganizationName("VulnScan");
    QCoreApplication::setApplicationName("VulnScan CLI");
    QCoreApplication::setApplicationVersion("0.1.0");

    // Initialize logger
    Logger::initialize("vulnscan_cli.log", true);
    Logger::info("VulnScan CLI starting...");

    // Load configuration
    Configuration& config = Configuration::instance();
    config.loadDefaults();

    // Parse command line arguments
    CommandLineParser parser;
    if (!parser.parse(app)) {
        Logger::error("Failed to parse command line arguments");
        Logger::shutdown();
        return 1;
    }

    QTextStream out(stdout);
    out << "VulnScan CLI v0.1.0" << Qt::endl;
    out << "===================" << Qt::endl;
    out << Qt::endl;
    out << "Target: " << parser.getTarget() << Qt::endl;
    out << "Port Range: " << parser.getPortRange() << Qt::endl;
    out << Qt::endl;
    out << "NOTE: Scanner implementation will be available in FASE 2" << Qt::endl;

    Logger::info("VulnScan CLI completed");
    Logger::shutdown();

    return 0;
}
