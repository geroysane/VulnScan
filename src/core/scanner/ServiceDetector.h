#ifndef SERVICEDETECTOR_H
#define SERVICEDETECTOR_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QMap>
#include "ServiceInfo.h"

/**
 * @class ServiceDetector
 * @brief Detects and fingerprints network services
 *
 * This class provides service detection capabilities through:
 * - Banner grabbing
 * - Service fingerprinting
 * - Version detection
 * - Protocol identification
 */
class ServiceDetector : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent QObject
     */
    explicit ServiceDetector(QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ServiceDetector() override;

    /**
     * @brief Detect service on a specific port
     * @param host Target host address
     * @param port Target port number
     * @param timeout Connection timeout in milliseconds
     * @return ServiceInfo structure with detected information
     */
    ServiceInfo detectService(const QString &host, int port, int timeout = 3000);

    /**
     * @brief Get service name by port number (well-known ports)
     * @param port Port number
     * @param protocol Protocol type (TCP/UDP)
     * @return Service name or empty string if unknown
     */
    QString getServiceByPort(int port, const QString &protocol = "TCP") const;

    /**
     * @brief Set custom timeout for service detection
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
     * @brief Emitted when service detection starts
     * @param host Target host
     * @param port Target port
     */
    void detectionStarted(const QString &host, int port);

    /**
     * @brief Emitted when service detection completes
     * @param serviceInfo Detected service information
     */
    void detectionCompleted(const ServiceInfo &serviceInfo);

    /**
     * @brief Emitted when service detection fails
     * @param host Target host
     * @param port Target port
     * @param error Error message
     */
    void detectionFailed(const QString &host, int port, const QString &error);

private:
    /**
     * @brief Grab banner from service
     * @param socket Connected socket
     * @param port Port number for service-specific probes
     * @return Banner string
     */
    QString grabBanner(QTcpSocket *socket, int port);

    /**
     * @brief Parse banner to extract service information
     * @param banner Raw banner string
     * @param port Port number
     * @return ServiceInfo with parsed data
     */
    ServiceInfo parseBanner(const QString &banner, int port);

    /**
     * @brief Get service-specific probe data
     * @param port Port number
     * @return Probe data to send to service
     */
    QByteArray getServiceProbe(int port);

    /**
     * @brief Initialize well-known port mappings
     */
    void initializePortMappings();

    int m_timeout;                           ///< Connection timeout in ms
    QMap<int, QString> m_tcpPortMap;        ///< TCP port to service mapping
    QMap<int, QString> m_udpPortMap;        ///< UDP port to service mapping
};

#endif // SERVICEDETECTOR_H
