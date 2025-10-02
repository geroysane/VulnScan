#include <QApplication>
#include <QStandardPaths>
#include "MainWindow.h"
#include "core/config/Configuration.h"
#include "core/common/GuiLogger.h"

/**
 * @brief Main entry point for VulnScan GUI application
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set application information
    QCoreApplication::setOrganizationName("VulnScan");
    QCoreApplication::setApplicationName("VulnScan GUI");
    QCoreApplication::setApplicationVersion("0.1.0");

    // Initialize GUI-safe logger (no qInstallMessageHandler)
    GuiLogger* logger = GuiLogger::instance();
    logger->setLogLevel(GuiLogger::Info);

    // Optional: Enable file logging
    QString logPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/vulnscan.log";
    logger->setFileLogging(true, logPath);

    logger->info("VulnScan GUI application started", "main");

    // Load configuration
    Configuration& config = Configuration::instance();
    config.loadDefaults();

    logger->info("Configuration loaded", "main");

    // Create and show main window
    MainWindow window;
    window.show();

    logger->info("Main window displayed", "main");

    int result = app.exec();

    logger->info("Application exiting", "main");

    return result;
}
