#ifndef SCANCONFIGURATION_H
#define SCANCONFIGURATION_H

#include <QString>
#include <QStringList>

/**
 * @class ScanConfiguration
 * @brief Configuration parameters for network scans
 *
 * This class encapsulates all configuration options for performing
 * network scans including targets, ports, timeouts, and scan types.
 */
class ScanConfiguration {
public:
    /**
     * @enum ScanType
     * @brief Type of scan to perform
     */
    enum ScanType {
        PortScan,           ///< Port scanning only
        ServiceDetection,   ///< Port scan + service detection
        FullScan            ///< Port scan + service detection + host discovery
    };

    /**
     * @brief Constructor
     */
    ScanConfiguration();

    // Target configuration
    QString target;                 ///< Target host or IP address
    QStringList targets;            ///< Multiple targets (for host discovery)

    // Port configuration
    int startPort;                  ///< Start of port range
    int endPort;                    ///< End of port range
    QList<int> specificPorts;       ///< Specific ports to scan (overrides range)

    // Scan behavior
    ScanType scanType;              ///< Type of scan to perform
    int timeout;                    ///< Connection timeout in milliseconds
    int maxThreads;                 ///< Maximum concurrent threads

    // Feature flags
    bool enableServiceDetection;    ///< Enable service fingerprinting
    bool enableDnsResolution;       ///< Enable DNS resolution for targets
    bool enableHostDiscovery;       ///< Enable host discovery mode

    /**
     * @brief Validate configuration
     * @return true if configuration is valid
     */
    bool isValid() const;

    /**
     * @brief Get total number of ports to scan
     * @return Number of ports
     */
    int getPortCount() const;

    /**
     * @brief Get list of all ports to scan
     * @return List of port numbers
     */
    QList<int> getPortsToScan() const;

    /**
     * @brief Create a quick scan configuration
     * @param target Target host
     * @return Pre-configured ScanConfiguration for quick scan
     */
    static ScanConfiguration quickScan(const QString &target);

    /**
     * @brief Create a full scan configuration
     * @param target Target host
     * @return Pre-configured ScanConfiguration for full scan
     */
    static ScanConfiguration fullScan(const QString &target);

    /**
     * @brief Create a custom port scan configuration
     * @param target Target host
     * @param startPort Start port
     * @param endPort End port
     * @return Pre-configured ScanConfiguration
     */
    static ScanConfiguration customScan(const QString &target, int startPort, int endPort);
};

#endif // SCANCONFIGURATION_H
