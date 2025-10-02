#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QHostAddress>
#include <QNetworkInterface>

/**
 * @struct HostInfo
 * @brief Information about a discovered host
 */
struct HostInfo {
    QString ipAddress;      ///< IP address
    QString hostname;       ///< Resolved hostname (if available)
    bool isAlive;          ///< Host is responding
    int responseTime;      ///< Response time in milliseconds (-1 if no response)

    HostInfo() : isAlive(false), responseTime(-1) {}
};

Q_DECLARE_METATYPE(HostInfo)

/**
 * @class NetworkInterface
 * @brief Network interface discovery and host enumeration
 *
 * This class provides network discovery capabilities:
 * - List available network interfaces
 * - Get interface details (IP, netmask, gateway)
 * - Host discovery (ping sweep)
 * - CIDR notation support
 * - Network range enumeration
 */
class NetworkInterface : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent QObject
     */
    explicit NetworkInterface(QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~NetworkInterface() override;

    /**
     * @brief Get list of all network interfaces
     * @return List of interface names
     */
    QStringList getInterfaces() const;

    /**
     * @brief Get details for a specific interface
     * @param interfaceName Interface name
     * @return QNetworkInterface object
     */
    QNetworkInterface getInterface(const QString &interfaceName) const;

    /**
     * @brief Get all IP addresses for an interface
     * @param interfaceName Interface name
     * @return List of IP addresses
     */
    QList<QHostAddress> getInterfaceAddresses(const QString &interfaceName) const;

    /**
     * @brief Expand CIDR notation to list of IP addresses
     * @param cidr CIDR notation (e.g., "192.168.1.0/24")
     * @return List of IP addresses in the range
     */
    QStringList expandCIDR(const QString &cidr) const;

    /**
     * @brief Expand IP range to list of addresses
     * @param startIp Start IP address
     * @param endIp End IP address
     * @return List of IP addresses
     */
    QStringList expandRange(const QString &startIp, const QString &endIp) const;

    /**
     * @brief Discover active hosts in a network range
     * @param targets List of IP addresses or CIDR ranges
     * @param timeout Timeout for each host in milliseconds
     * @return List of discovered hosts
     */
    QList<HostInfo> discoverHosts(const QStringList &targets, int timeout = 1000);

    /**
     * @brief Check if a single host is alive (TCP ping)
     * @param host IP address or hostname
     * @param port Port to check (default 80)
     * @param timeout Timeout in milliseconds
     * @return true if host responds
     */
    bool isHostAlive(const QString &host, int port = 80, int timeout = 1000);

    /**
     * @brief Get local IP address (first non-loopback IPv4)
     * @return Local IP address or empty string
     */
    QString getLocalIpAddress() const;

    /**
     * @brief Get default gateway for an interface
     * @param interfaceName Interface name (empty for default)
     * @return Gateway IP address
     */
    QString getGateway(const QString &interfaceName = QString()) const;

signals:
    /**
     * @brief Emitted when host discovery starts
     * @param totalHosts Total number of hosts to check
     */
    void discoveryStarted(int totalHosts);

    /**
     * @brief Emitted for each discovered host
     * @param hostInfo Information about the discovered host
     */
    void hostDiscovered(const HostInfo &hostInfo);

    /**
     * @brief Emitted when host discovery completes
     * @param hostsFound Number of active hosts found
     */
    void discoveryCompleted(int hostsFound);

    /**
     * @brief Emitted to report discovery progress
     * @param current Current host index
     * @param total Total hosts to check
     */
    void discoveryProgress(int current, int total);

private:
    /**
     * @brief Parse CIDR notation
     * @param cidr CIDR string
     * @param network Output: network address
     * @param prefixLength Output: prefix length
     * @return true if parsing succeeded
     */
    bool parseCIDR(const QString &cidr, QHostAddress &network, int &prefixLength) const;

    /**
     * @brief Calculate network address from IP and prefix
     * @param address IP address
     * @param prefixLength Prefix length
     * @return Network address
     */
    QHostAddress getNetworkAddress(const QHostAddress &address, int prefixLength) const;

    /**
     * @brief Calculate broadcast address
     * @param network Network address
     * @param prefixLength Prefix length
     * @return Broadcast address
     */
    QHostAddress getBroadcastAddress(const QHostAddress &network, int prefixLength) const;

    /**
     * @brief Convert IP address to 32-bit integer
     * @param address IP address
     * @return 32-bit representation
     */
    quint32 ipToUInt32(const QHostAddress &address) const;

    /**
     * @brief Convert 32-bit integer to IP address
     * @param ip 32-bit representation
     * @return IP address
     */
    QHostAddress uInt32ToIp(quint32 ip) const;
};

#endif // NETWORKINTERFACE_H
