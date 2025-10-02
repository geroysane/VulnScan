#ifndef CLIAPPLICATION_H
#define CLIAPPLICATION_H

#include <QObject>
#include <QCoreApplication>
#include <QSharedPointer>
#include "../core/scanner/ScanEngine.h"
#include "../core/scanner/ScanConfiguration.h"
#include "../core/database/ScanRepository.h"
#include "../core/database/DatabaseManager.h"

/**
 * @class CliApplication
 * @brief Command-line interface application for VulnScan
 *
 * This class provides CLI functionality for:
 * - Performing network scans
 * - Listing scan history
 * - Viewing scan results
 * - Managing scan database
 */
class CliApplication : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent QObject
     */
    explicit CliApplication(QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~CliApplication() override;

    /**
     * @brief Initialize the application
     * @param argc Argument count
     * @param argv Argument values
     * @return true if initialization succeeded
     */
    bool initialize(int argc, char *argv[]);

    /**
     * @brief Run the application
     * @return Exit code
     */
    int run();

private slots:
    /**
     * @brief Handle scan completion
     * @param result Scan result
     */
    void handleScanCompleted(const ScanResult &result);

    /**
     * @brief Handle scan failure
     * @param error Error message
     */
    void handleScanFailed(const QString &error);

    /**
     * @brief Handle scan progress
     * @param progress Progress percentage (0-100)
     * @param message Status message
     */
    void handleScanProgress(int progress, const QString &message);

private:
    /**
     * @brief Parse command line arguments
     * @return true if parsing succeeded
     */
    bool parseArguments();

    /**
     * @brief Show help message
     */
    void showHelp();

    /**
     * @brief Perform a network scan
     * @return Exit code
     */
    int performScan();

    /**
     * @brief List recent scans
     * @return Exit code
     */
    int listScans();

    /**
     * @brief Show details of a specific scan
     * @param scanId Scan ID
     * @return Exit code
     */
    int showScan(int scanId);

    /**
     * @brief Delete a scan
     * @param scanId Scan ID
     * @return Exit code
     */
    int deleteScan(int scanId);

    /**
     * @brief Delete old scans
     * @param days Delete scans older than this many days
     * @return Exit code
     */
    int deleteOldScans(int days);

    /**
     * @brief Print scan result
     * @param result Scan result to print
     */
    void printScanResult(const ScanResult &result);

    /**
     * @brief Print scan summary
     * @param result Scan result
     */
    void printScanSummary(const ScanResult &result);

    int m_argc;
    char **m_argv;
    QCoreApplication *m_app;

    QString m_command;
    QString m_target;
    int m_startPort;
    int m_endPort;
    int m_scanId;
    int m_days;
    bool m_enableServiceDetection;
    bool m_enableDnsResolution;

    QSharedPointer<ScanEngine> m_scanEngine;
    QSharedPointer<ScanRepository> m_repository;
};

#endif // CLIAPPLICATION_H
