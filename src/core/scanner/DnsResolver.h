#ifndef DNSRESOLVER_H
#define DNSRESOLVER_H

#include <QObject>
#include <QString>
#include <QHostInfo>
#include <QList>
#include <QHostAddress>
#include <QMap>

/**
 * @class DnsResolver
 * @brief DNS resolution and reverse lookup utilities
 *
 * This class provides DNS resolution capabilities:
 * - Forward DNS lookup (hostname to IP)
 * - Reverse DNS lookup (IP to hostname)
 * - Asynchronous and synchronous resolution
 * - Multiple address resolution
 */
class DnsResolver : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent QObject
     */
    explicit DnsResolver(QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~DnsResolver() override;

    /**
     * @brief Resolve hostname to IP addresses (synchronous)
     * @param hostname Hostname to resolve
     * @param timeout Timeout in milliseconds
     * @return List of IP addresses
     */
    QList<QHostAddress> resolveHost(const QString &hostname, int timeout = 5000);

    /**
     * @brief Reverse lookup - resolve IP to hostname (synchronous)
     * @param address IP address to resolve
     * @param timeout Timeout in milliseconds
     * @return Hostname or empty string if resolution fails
     */
    QString reverseResolve(const QString &address, int timeout = 5000);

    /**
     * @brief Start asynchronous hostname resolution
     * @param hostname Hostname to resolve
     * @return Lookup ID for tracking
     */
    int resolveHostAsync(const QString &hostname);

    /**
     * @brief Start asynchronous reverse lookup
     * @param address IP address to resolve
     * @return Lookup ID for tracking
     */
    int reverseResolveAsync(const QString &address);

    /**
     * @brief Cancel pending lookup
     * @param lookupId Lookup ID to cancel
     */
    void cancelLookup(int lookupId);

    /**
     * @brief Set default timeout for synchronous operations
     * @param timeout Timeout in milliseconds
     */
    void setTimeout(int timeout);

    /**
     * @brief Get current timeout setting
     * @return Timeout in milliseconds
     */
    int getTimeout() const;

signals:
    /**
     * @brief Emitted when async hostname resolution completes
     * @param lookupId Lookup ID
     * @param hostname Original hostname
     * @param addresses Resolved IP addresses
     */
    void hostResolved(int lookupId, const QString &hostname, const QList<QHostAddress> &addresses);

    /**
     * @brief Emitted when async reverse lookup completes
     * @param lookupId Lookup ID
     * @param address Original IP address
     * @param hostname Resolved hostname
     */
    void reverseResolved(int lookupId, const QString &address, const QString &hostname);

    /**
     * @brief Emitted when resolution fails
     * @param lookupId Lookup ID
     * @param error Error message
     */
    void resolutionFailed(int lookupId, const QString &error);

private slots:
    /**
     * @brief Handle async lookup completion
     * @param hostInfo Resolution result
     */
    void handleLookupResult(const QHostInfo &hostInfo);

private:
    /**
     * @brief Validate hostname format
     * @param hostname Hostname to validate
     * @return true if valid
     */
    bool isValidHostname(const QString &hostname) const;

    /**
     * @brief Validate IP address format
     * @param address IP address to validate
     * @return true if valid
     */
    bool isValidIpAddress(const QString &address) const;

    int m_timeout;                              ///< Default timeout in ms
    QMap<int, QString> m_pendingLookups;       ///< Map of lookup ID to hostname/IP
    QMap<int, bool> m_isReverseLookup;         ///< Track if lookup is reverse
};

#endif // DNSRESOLVER_H
