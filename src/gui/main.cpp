#include <QApplication>
#include "MainWindow.h"
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

    // Note: Logger disabled for GUI due to qInstallMessageHandler issues
    // Will be reimplemented in FASE 3 with GUI-safe logging

    // Load configuration
    Configuration& config = Configuration::instance();
    config.loadDefaults();

    // Create and show main window
    MainWindow window;
    window.show();

    return app.exec();
}
