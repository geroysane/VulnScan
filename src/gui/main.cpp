#include <QApplication>
#include "MainWindow.h"
#include "core/common/Logger.h"
#include "core/config/Configuration.h"

/**
 * @brief Main entry point for VulnScan GUI application
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set application information
    QCoreApplication::setOrganizationName("VulnScan");
    QCoreApplication::setApplicationName("VulnScan GUI");
    QCoreApplication::setApplicationVersion("0.1.0");

    // Initialize logger
    Logger::initialize("vulnscan_gui.log", true);
    Logger::info("VulnScan GUI starting...");

    // Load configuration
    Configuration& config = Configuration::instance();
    config.loadDefaults();

    // Create and show main window
    MainWindow window;
    window.show();

    Logger::info("VulnScan GUI started successfully");

    int result = app.exec();

    // Cleanup
    Logger::info("VulnScan GUI shutting down...");
    Logger::shutdown();

    return result;
}
