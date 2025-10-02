#ifndef SCANENGINE_H
#define SCANENGINE_H

#include <QObject>
#include <QSharedPointer>
#include "ScanConfiguration.h"
#include "ScanResult.h"
#include "PortScanner.h"
#include "ServiceDetector.h"
#include "DnsResolver.h"
#include "NetworkInterface.h"

/**
 * @class ScanEngine
 * @brief Central orchestration engine for network scans
 *
 * This class coordinates all scanning components to perform complete
 * network scans including port scanning, service detection, DNS resolution,
 * and host discovery.
 */
class ScanEngine : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent QObject
     */
    explicit ScanEngine(QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ScanEngine() override;

    /**
     * @brief Start a scan with the given configuration
     * @param config Scan configuration
     * @return true if scan started successfully
     */
    bool startScan(const ScanConfiguration &config);

    /**
     * @brief Cancel the current scan
     */
    void cancelScan();

    /**
     * @brief Check if a scan is currently running
     * @return true if scan is in progress
     */
    bool isScanning() const;

    /**
     * @brief Get the current scan result
     * @return Current scan result
     */
    ScanResult getCurrentResult() const;

    /**
     * @brief Get the current configuration
     * @return Current scan configuration
     */
    ScanConfiguration getConfiguration() const;

signals:
    /**
     * @brief Emitted when scan starts
     * @param target Target being scanned
     */
    void scanStarted(const QString &target);

    /**
     * @brief Emitted when scan completes successfully
     * @param result Final scan result
     */
    void scanCompleted(const ScanResult &result);

    /**
     * @brief Emitted when scan fails or is cancelled
     * @param error Error message
     */
    void scanFailed(const QString &error);

    /**
     * @brief Emitted to report overall scan progress
     * @param current Current progress (0-100)
     * @param message Status message
     */
    void scanProgress(int current, const QString &message);

    /**
     * @brief Emitted when a port is found open
     * @param portResult Port scan result
     */
    void portDiscovered(const PortResult &portResult);

    /**
     * @brief Emitted when a service is detected
     * @param serviceInfo Service information
     */
    void serviceDetected(const ServiceInfo &serviceInfo);

private slots:
    /**
     * @brief Handle port scanner progress
     * @param current Current progress
     * @param total Total ports
     */
    void handlePortScanProgress(int current, int total);

    /**
     * @brief Handle port found event
     * @param result Port result
     */
    void handlePortFound(const PortResult &result);

    /**
     * @brief Handle port scan completion
     * @param results List of port results
     */
    void handlePortScanCompleted(const QList<PortResult> &results);

    /**
     * @brief Handle service detection completion
     * @param serviceInfo Detected service info
     */
    void handleServiceDetected(const ServiceInfo &serviceInfo);

    /**
     * @brief Handle DNS resolution completion
     * @param lookupId Lookup ID
     * @param hostname Original hostname
     * @param addresses Resolved addresses
     */
    void handleDnsResolved(int lookupId, const QString &hostname, const QList<QHostAddress> &addresses);

private:
    /**
     * @brief Perform DNS resolution if enabled
     */
    void performDnsResolution();

    /**
     * @brief Perform port scanning
     */
    void performPortScan();

    /**
     * @brief Perform service detection on discovered ports
     */
    void performServiceDetection();

    /**
     * @brief Finalize scan and emit results
     */
    void finalizeScan();

    /**
     * @brief Calculate overall progress percentage
     * @return Progress percentage (0-100)
     */
    int calculateProgress() const;

    ScanConfiguration m_config;                     ///< Current configuration
    ScanResult m_result;                            ///< Current scan result
    bool m_isScanning;                              ///< Scan in progress flag

    QSharedPointer<PortScanner> m_portScanner;      ///< Port scanner instance
    QSharedPointer<ServiceDetector> m_serviceDetector; ///< Service detector instance
    QSharedPointer<DnsResolver> m_dnsResolver;      ///< DNS resolver instance
    QSharedPointer<NetworkInterface> m_networkInterface; ///< Network interface instance

    int m_totalPorts;                               ///< Total ports to scan
    int m_scannedPorts;                             ///< Ports scanned so far
    int m_servicesDetected;                         ///< Services detected count
};

#endif // SCANENGINE_H
