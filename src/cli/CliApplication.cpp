#include "CliApplication.h"
#include <QCommandLineParser>
#include <QTextStream>
#include <QDir>
#include <QTimer>
#include <iostream>

CliApplication::CliApplication(QObject *parent)
    : QObject(parent)
    , m_argc(0)
    , m_argv(nullptr)
    , m_app(nullptr)
    , m_startPort(1)
    , m_endPort(1000)
    , m_scanId(0)
    , m_days(30)
    , m_enableServiceDetection(false)
    , m_enableDnsResolution(false)
{
}

CliApplication::~CliApplication()
{
}

bool CliApplication::initialize(int argc, char *argv[])
{
    m_argc = argc;
    m_argv = argv;
    m_app = new QCoreApplication(argc, argv);

    m_app->setApplicationName("VulnScan");
    m_app->setApplicationVersion("1.0.0");

    // Initialize database
    QString dbPath = QDir::homePath() + "/.vulnscan/scans.db";
    QDir().mkpath(QDir::homePath() + "/.vulnscan");

    if (!DatabaseManager::instance().initialize(dbPath)) {
        std::cerr << "Failed to initialize database" << std::endl;
        return false;
    }

    // Create repository and scan engine
    m_repository = QSharedPointer<ScanRepository>::create();
    m_scanEngine = QSharedPointer<ScanEngine>::create();

    // Connect signals
    connect(m_scanEngine.data(), &ScanEngine::scanCompleted,
            this, &CliApplication::handleScanCompleted);
    connect(m_scanEngine.data(), &ScanEngine::scanFailed,
            this, &CliApplication::handleScanFailed);
    connect(m_scanEngine.data(), &ScanEngine::scanProgress,
            this, &CliApplication::handleScanProgress);

    return parseArguments();
}

int CliApplication::run()
{
    if (m_command == "scan") {
        return performScan();
    } else if (m_command == "list") {
        return listScans();
    } else if (m_command == "show") {
        return showScan(m_scanId);
    } else if (m_command == "delete") {
        return deleteScan(m_scanId);
    } else if (m_command == "clean") {
        return deleteOldScans(m_days);
    } else {
        showHelp();
        return 1;
    }
}

bool CliApplication::parseArguments()
{
    QCommandLineParser parser;
    parser.setApplicationDescription("VulnScan - Network Vulnerability Scanner");
    parser.addHelpOption();
    parser.addVersionOption();

    // Commands
    parser.addPositionalArgument("command", "Command to execute: scan, list, show, delete, clean");

    // Scan options
    QCommandLineOption targetOption(QStringList() << "t" << "target",
                                     "Target host or IP address", "target");
    parser.addOption(targetOption);

    QCommandLineOption portsOption(QStringList() << "p" << "ports",
                                    "Port range (e.g., 1-1000)", "ports", "1-1000");
    parser.addOption(portsOption);

    QCommandLineOption serviceOption(QStringList() << "s" << "service-detection",
                                      "Enable service detection");
    parser.addOption(serviceOption);

    QCommandLineOption dnsOption(QStringList() << "d" << "dns-resolution",
                                  "Enable DNS resolution");
    parser.addOption(dnsOption);

    // Show/Delete options
    QCommandLineOption idOption(QStringList() << "i" << "id",
                                "Scan ID", "id");
    parser.addOption(idOption);

    // Clean options
    QCommandLineOption daysOption(QStringList() << "days",
                                   "Delete scans older than X days", "days", "30");
    parser.addOption(daysOption);

    // Process arguments
    parser.process(*m_app);

    // Get command
    const QStringList args = parser.positionalArguments();
    if (args.isEmpty()) {
        showHelp();
        return false;
    }

    m_command = args.first();

    // Parse command-specific options
    if (m_command == "scan") {
        if (!parser.isSet(targetOption)) {
            std::cerr << "Error: --target is required for scan command" << std::endl;
            return false;
        }
        m_target = parser.value(targetOption);

        // Parse port range
        QString portRange = parser.value(portsOption);
        QStringList portParts = portRange.split('-');
        if (portParts.size() == 2) {
            m_startPort = portParts[0].toInt();
            m_endPort = portParts[1].toInt();
        }

        m_enableServiceDetection = parser.isSet(serviceOption);
        m_enableDnsResolution = parser.isSet(dnsOption);

    } else if (m_command == "show" || m_command == "delete") {
        if (!parser.isSet(idOption)) {
            std::cerr << "Error: --id is required for " << m_command.toStdString() << " command" << std::endl;
            return false;
        }
        m_scanId = parser.value(idOption).toInt();

    } else if (m_command == "clean") {
        if (parser.isSet(daysOption)) {
            m_days = parser.value(daysOption).toInt();
        }
    }

    return true;
}

void CliApplication::showHelp()
{
    std::cout << "VulnScan - Network Vulnerability Scanner\n\n";
    std::cout << "Usage: vulnscan-cli <command> [options]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  scan              Perform a network scan\n";
    std::cout << "  list              List recent scans\n";
    std::cout << "  show              Show scan details\n";
    std::cout << "  delete            Delete a scan\n";
    std::cout << "  clean             Delete old scans\n\n";
    std::cout << "Scan Options:\n";
    std::cout << "  -t, --target      Target host or IP (required)\n";
    std::cout << "  -p, --ports       Port range (default: 1-1000)\n";
    std::cout << "  -s, --service-detection    Enable service detection\n";
    std::cout << "  -d, --dns-resolution       Enable DNS resolution\n\n";
    std::cout << "Show/Delete Options:\n";
    std::cout << "  -i, --id          Scan ID (required)\n\n";
    std::cout << "Clean Options:\n";
    std::cout << "  --days            Delete scans older than X days (default: 30)\n\n";
    std::cout << "Examples:\n";
    std::cout << "  vulnscan-cli scan -t 192.168.1.1 -p 1-1000\n";
    std::cout << "  vulnscan-cli scan -t example.com -p 80-443 -s -d\n";
    std::cout << "  vulnscan-cli list\n";
    std::cout << "  vulnscan-cli show -i 42\n";
    std::cout << "  vulnscan-cli delete -i 42\n";
    std::cout << "  vulnscan-cli clean --days 7\n";
}

int CliApplication::performScan()
{
    std::cout << "Starting scan of " << m_target.toStdString() << "..." << std::endl;

    // Create scan configuration
    ScanConfiguration config = ScanConfiguration::customScan(m_target, m_startPort, m_endPort);
    config.enableServiceDetection = m_enableServiceDetection;
    config.enableDnsResolution = m_enableDnsResolution;

    // Start scan after event loop starts (using QTimer::singleShot)
    QTimer::singleShot(0, this, [this, config]() {
        if (!m_scanEngine->startScan(config)) {
            std::cerr << "Failed to start scan" << std::endl;
            m_app->exit(1);
        }
    });

    // Run event loop until scan completes
    return m_app->exec();
}

int CliApplication::listScans()
{
    QList<ScanResult> scans = m_repository->findRecent(20);

    if (scans.isEmpty()) {
        std::cout << "No scans found." << std::endl;
        return 0;
    }

    std::cout << "\nRecent Scans:\n";
    std::cout << "============================================================\n";
    std::cout << "ID\tTarget\t\t\tPorts\tStatus\t\tDate\n";
    std::cout << "============================================================\n";

    for (const ScanResult &scan : scans) {
        int openPorts = 0;
        for (const PortResult &port : scan.ports) {
            if (port.state == "open") {
                openPorts++;
            }
        }

        std::cout << scan.id << "\t"
                  << scan.targetHost.toStdString() << "\t\t"
                  << openPorts << "\t"
                  << scan.status.toStdString() << "\t\t"
                  << scan.scanStartTime.toString("yyyy-MM-dd HH:mm").toStdString()
                  << std::endl;
    }

    std::cout << std::endl;
    return 0;
}

int CliApplication::showScan(int scanId)
{
    std::optional<ScanResult> scanOpt = m_repository->findById(scanId);

    if (!scanOpt.has_value()) {
        std::cerr << "Scan not found: " << scanId << std::endl;
        return 1;
    }

    printScanResult(scanOpt.value());
    return 0;
}

int CliApplication::deleteScan(int scanId)
{
    m_repository->remove(scanId);
    std::cout << "Scan " << scanId << " deleted successfully." << std::endl;
    return 0;
}

int CliApplication::deleteOldScans(int days)
{
    int deleted = m_repository->deleteOlderThan(days);
    std::cout << "Deleted " << deleted << " scans older than " << days << " days." << std::endl;
    return 0;
}

void CliApplication::printScanResult(const ScanResult &result)
{
    std::cout << "\n============================================================\n";
    std::cout << "Scan ID: " << result.id << "\n";
    std::cout << "Target: " << result.targetHost.toStdString() << "\n";
    std::cout << "Status: " << result.status.toStdString() << "\n";
    std::cout << "Started: " << result.scanStartTime.toString("yyyy-MM-dd HH:mm:ss").toStdString() << "\n";
    std::cout << "Ended: " << result.scanEndTime.toString("yyyy-MM-dd HH:mm:ss").toStdString() << "\n";
    std::cout << "============================================================\n\n";

    if (result.ports.isEmpty()) {
        std::cout << "No ports found.\n";
        return;
    }

    std::cout << "Open Ports:\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "Port\tState\tService\t\tVersion\n";
    std::cout << "------------------------------------------------------------\n";

    for (const PortResult &port : result.ports) {
        if (port.state == "open") {
            std::cout << port.port << "\t"
                      << port.state.toStdString() << "\t"
                      << port.service.toStdString() << "\t\t"
                      << port.version.toStdString() << std::endl;
        }
    }

    std::cout << "\n";
}

void CliApplication::printScanSummary(const ScanResult &result)
{
    int openPorts = 0;
    for (const PortResult &port : result.ports) {
        if (port.state == "open") {
            openPorts++;
        }
    }

    std::cout << "\nScan completed successfully!" << std::endl;
    std::cout << "Target: " << result.targetHost.toStdString() << std::endl;
    std::cout << "Open ports: " << openPorts << "/" << result.ports.size() << std::endl;
    std::cout << "Duration: " << result.scanStartTime.secsTo(result.scanEndTime) << " seconds" << std::endl;
}

void CliApplication::handleScanCompleted(const ScanResult &result)
{
    // Save scan to database
    m_repository->save(result);
    printScanSummary(result);
    printScanResult(result);

    // Exit application
    m_app->quit();
}

void CliApplication::handleScanFailed(const QString &error)
{
    std::cerr << "\nScan failed: " << error.toStdString() << std::endl;
    m_app->exit(1);
}

void CliApplication::handleScanProgress(int progress, const QString &message)
{
    std::cout << "\r[" << progress << "%] " << message.toStdString() << std::flush;
}
